#!/usr/bin/env bash
# Measures line coverage of the core library (src/ without main.cpp) after a coverage build.
# Usage: scripts/check-coverage.sh [build-dir]   (default: build/coverage)
set -euo pipefail
cd "$(dirname "$0")/.."

BUILD_DIR="${1:-build/coverage}"
THRESHOLD="${COVERAGE_THRESHOLD:-90}"

gcovr --root . --object-directory "$BUILD_DIR" \
      --filter 'src/.*' --filter 'include/.*' --exclude 'src/main\.cpp' \
      --print-summary --fail-under-line "$THRESHOLD"
