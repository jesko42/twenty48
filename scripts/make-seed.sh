#!/usr/bin/env bash
# Builds the "seed" directory: only the files the team brings to a fresh repo.
# Everything else is produced live by Claude Code following CLAUDE.md and SPEC.md.
# Usage: scripts/make-seed.sh [target-dir]   (default: ../twenty48-seed)
set -euo pipefail
cd "$(dirname "$0")/.."

TARGET="${1:-../twenty48-seed}"
rm -rf "$TARGET"
mkdir -p "$TARGET/third_party/doctest" "$TARGET/tests"

cp CLAUDE.md SPEC.md .clang-format .clang-tidy .gitignore .editorconfig LICENSE "$TARGET/"
cp tests/.clang-tidy "$TARGET/tests/"
cp third_party/doctest/doctest.h third_party/doctest/LICENSE.txt "$TARGET/third_party/doctest/"

# Reset the spec to its first version: the undo requirement is added live.
sed -i '/R11 Rückgängig, R10.1 Taste/d' "$TARGET/SPEC.md"

echo "seed written to $TARGET:"
(cd "$TARGET" && find . -type f | sort)
