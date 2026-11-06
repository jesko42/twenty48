#!/usr/bin/env bash
# Fails if any project source file deviates from .clang-format.
# Usage: scripts/check-format.sh [--fix]
set -euo pipefail
cd "$(dirname "$0")/.."

CLANG_FORMAT="${CLANG_FORMAT:-$(command -v clang-format-18 || command -v clang-format)}"
mapfile -t files < <(git ls-files 'include/*.hpp' 'src/*.cpp' 'tests/*.cpp' 'tests/*.hpp')

if [[ "${1:-}" == "--fix" ]]; then
    "$CLANG_FORMAT" -i "${files[@]}"
    echo "formatted ${#files[@]} files"
else
    "$CLANG_FORMAT" --dry-run --Werror "${files[@]}"
    echo "format OK (${#files[@]} files)"
fi
