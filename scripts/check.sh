#!/usr/bin/env bash
# The local quality gate: exactly what CI runs, in one command.
set -euo pipefail
cd "$(dirname "$0")/.."

step() { printf '\n\033[1;34m==> %s\033[0m\n' "$*"; }

step "format";     scripts/check-format.sh
step "configure";  cmake --preset dev > /dev/null
step "build";      cmake --build --preset dev
step "test";       ctest --preset dev
step "clang-tidy"; scripts/check-tidy.sh build/dev
printf '\n\033[1;32mAll checks passed.\033[0m\n'
