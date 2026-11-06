#!/usr/bin/env bash
# Resets the working copy to the state the talk starts from and warms the build cache.
# Safe to run as often as you like before and between rehearsals.
set -euo pipefail
cd "$(dirname "$0")/.."

git checkout -q main
git branch --list 'live/*' | xargs -r git branch -D
git clean -fdq -e build/
cmake --preset dev > /dev/null
cmake --build --preset dev > /dev/null
ctest --preset dev > /dev/null
echo "ready: on main, build warm, tests green"
git log --oneline -3
