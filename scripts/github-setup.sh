#!/usr/bin/env bash
# One-time setup of the GitHub repository: create, push, protect main, enable Actions.
# Usage: scripts/github-setup.sh <owner>/<repo> [--private]
set -euo pipefail
cd "$(dirname "$0")/.."

REPO="${1:?usage: scripts/github-setup.sh <owner>/<repo> [--private]}"
VISIBILITY="${2:---public}"
OWNER="${REPO%%/*}"

gh auth status >/dev/null

echo "==> create and push $REPO"
gh repo create "$REPO" "$VISIBILITY" --source . --remote origin --push \
    --description "2048 in C++17, spec-driven, with Claude Code"
git push -u origin --all

echo "==> replace CODEOWNERS placeholder with @$OWNER"
sed -i "s/@OWNER/@$OWNER/" .github/CODEOWNERS
git add .github/CODEOWNERS
git commit -q -m "chore: set code owner" || true
git push -q

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
