#!/usr/bin/env bash
# One-time setup of the GitHub repository: create, push, protect main, enable Actions.
# Safe to re-run. Usage: scripts/github-setup.sh <owner>/<repo> [--private]
set -euo pipefail
cd "$(dirname "$0")/.."

REPO="${1:?usage: scripts/github-setup.sh <owner>/<repo> [--private]}"
VISIBILITY="${2:---public}"
OWNER="${REPO%%/*}"

gh auth status >/dev/null

# Remote name: "origin" unless origin already points somewhere else (e.g. a private server),
# then the GitHub remote is called "github".
REMOTE="origin"
if git remote get-url origin >/dev/null 2>&1 && ! git remote get-url origin | grep -q "github.com[:/]$REPO"; then
    REMOTE="github"
fi

if gh repo view "$REPO" >/dev/null 2>&1; then
    echo "==> $REPO already exists on GitHub"
else
    echo "==> create $REPO"
    gh repo create "$REPO" "$VISIBILITY" --description "2048 in C++17, spec-driven, with Claude Code"
fi

# Prefer SSH: pushing workflow files over HTTPS needs the extra "workflow" token scope.
SSH_CHECK="$(ssh -o BatchMode=yes -o ConnectTimeout=10 -T git@github.com 2>&1 || true)"
if grep -q "successfully authenticated" <<<"$SSH_CHECK"; then
    URL="git@github.com:$REPO.git"
else
    gh auth setup-git >/dev/null
    URL="https://github.com/$REPO.git"
fi
if git remote get-url "$REMOTE" >/dev/null 2>&1; then
    git remote set-url "$REMOTE" "$URL"
else
    git remote add "$REMOTE" "$URL"
fi
gh repo set-default "$REPO"

echo "==> push all branches to $REMOTE"
git push -u "$REMOTE" --all

echo "==> replace CODEOWNERS placeholder with @$OWNER"
sed -i "s/@OWNER/@$OWNER/" .github/CODEOWNERS
git add .github/CODEOWNERS
git commit -q -m "chore: set code owner" || true
git push -q "$REMOTE" main

echo "==> protect main (PR required, all CI checks required, no force push)"
gh api -X PUT "repos/$REPO/branches/main/protection" \
    -H "Accept: application/vnd.github+json" \
    --input - <<'JSON'
{
  "required_status_checks": {
    "strict": true,
    "contexts": ["build-test (gcc)", "build-test (clang)", "format", "tidy", "coverage"]
  },
  "enforce_admins": false,
  "required_pull_request_reviews": { "required_approving_review_count": 0 },
  "restrictions": null,
  "allow_force_pushes": false,
  "allow_deletions": false,
  "required_linear_history": true
}
JSON

echo "==> auto-delete merged branches, squash merges only"
gh api -X PATCH "repos/$REPO" -f delete_branch_on_merge=true -f allow_merge_commit=false \
    -f allow_rebase_merge=false -f allow_squash_merge=true >/dev/null

echo "==> trigger CI once so the required checks exist"
gh workflow run ci.yml --ref main 2>/dev/null || true

echo
echo "Done. Open: https://github.com/$REPO"
