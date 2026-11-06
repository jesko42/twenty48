#pragma once

#include <vector>

namespace twenty48 {

/// One line of the board. Index 0 is the side the tiles slide towards.
using Row = std::vector<int>;

struct SlideResult {
    Row row;
    int score = 0;         ///< Sum of all tiles created by merges in this slide (SPEC R3).
    bool changed = false;  ///< True if the row differs from the input (SPEC R6).
};

/// Slides all tiles towards index 0 and merges equal neighbours exactly once (SPEC R2, R3).
[[nodiscard]] SlideResult slideLeft(const Row& row);

}  // namespace twenty48
