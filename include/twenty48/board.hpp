#pragma once

#include <cstddef>
#include <vector>

namespace twenty48 {

enum class Direction { Left, Right, Up, Down };

struct Cell {
    int row = 0;
    int col = 0;
};

struct MoveResult {
    int score = 0;
    bool changed = false;
};

/// Square grid of tiles. Value 0 means empty. Pure logic, no randomness.
class Board {
public:
    static constexpr int kDefaultSize = 4;
    static constexpr int kMinSize = 2;

    /// @throws std::invalid_argument if size < kMinSize (SPEC R1).
    explicit Board(int size = kDefaultSize);

    [[nodiscard]] int size() const noexcept { return size_; }

    /// @throws std::out_of_range for coordinates outside the board.
    [[nodiscard]] int at(int row, int col) const;
    void set(int row, int col, int value);

    /// Slides and merges every line in the given direction (SPEC R2, R3, R6).
    [[nodiscard]] MoveResult move(Direction dir);

    [[nodiscard]] std::vector<Cell> emptyCells() const;
    [[nodiscard]] bool canMove() const;  ///< SPEC R9
    [[nodiscard]] int maxTile() const;

    [[nodiscard]] bool operator==(const Board& other) const noexcept;
    [[nodiscard]] bool operator!=(const Board& other) const noexcept;

private:
    /// Maps (line, offset) to a cell such that offset 0 is the wall the tiles slide towards.
    [[nodiscard]] Cell lineCell(Direction dir, int line, int offset) const noexcept;
    [[nodiscard]] std::size_t index(int row, int col) const;

    int size_;
    std::vector<int> cells_;
};

}  // namespace twenty48
