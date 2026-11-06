#!/usr/bin/env bash
# Runs clang-tidy over the project sources using the compile database of a configured build.
# Usage: scripts/check-tidy.sh [build-dir]   (default: build/dev)
set -euo pipefail
cd "$(dirname "$0")/.."

BUILD_DIR="${1:-build/dev}"
CLANG_TIDY="${CLANG_TIDY:-$(command -v clang-tidy-18 || command -v clang-tidy)}"
[[ -f "$BUILD_DIR/compile_commands.json" ]] || { echo "no compile database in $BUILD_DIR, run: cmake --preset dev"; exit 1; }

mapfile -t files < <(git ls-files 'src/*.cpp' 'tests/*.cpp')
"$CLANG_TIDY" -p "$BUILD_DIR" --quiet "${files[@]}"
echo "tidy OK (${#files[@]} files)"
