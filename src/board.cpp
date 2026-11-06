#include "twenty48/board.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

#include "twenty48/row.hpp"

namespace twenty48 {

Board::Board(int size) : size_(size) {
    if (size < kMinSize) {
        throw std::invalid_argument("board size must be at least " + std::to_string(kMinSize));
    }
    cells_.assign(static_cast<std::size_t>(size) * static_cast<std::size_t>(size), 0);
}

std::size_t Board::index(int row, int col) const {
    if (row < 0 || row >= size_ || col < 0 || col >= size_) {
        throw std::out_of_range("cell (" + std::to_string(row) + ", " + std::to_string(col) +
                                ") is outside a " + std::to_string(size_) + "x" +
                                std::to_string(size_) + " board");
    }
    return static_cast<std::size_t>(row) * static_cast<std::size_t>(size_) +
           static_cast<std::size_t>(col);
}

int Board::at(int row, int col) const {
    return cells_[index(row, col)];
}

void Board::set(int row, int col, int value) {
    cells_[index(row, col)] = value;
}

Cell Board::lineCell(Direction dir, int line, int offset) const noexcept {
    const int mirrored = size_ - 1 - offset;
    switch (dir) {
        case Direction::Left:
            return {line, offset};
        case Direction::Right:
            return {line, mirrored};
        case Direction::Up:
            return {offset, line};
        case Direction::Down:
            return {mirrored, line};
    }
    return {line, offset};  // unreachable, keeps -Wreturn-type quiet
}

MoveResult Board::move(Direction dir) {
    MoveResult result;
    Row line(static_cast<std::size_t>(size_));

    for (int i = 0; i < size_; ++i) {
        for (int j = 0; j < size_; ++j) {
            const Cell cell = lineCell(dir, i, j);
            line[static_cast<std::size_t>(j)] = at(cell.row, cell.col);
        }
        const SlideResult slid = slideLeft(line);
        for (int j = 0; j < size_; ++j) {
            const Cell cell = lineCell(dir, i, j);
            set(cell.row, cell.col, slid.row[static_cast<std::size_t>(j)]);
        }
        result.score += slid.score;
        result.changed = result.changed || slid.changed;
    }
    return result;
}

std::vector<Cell> Board::emptyCells() const {
    std::vector<Cell> empty;
    for (int row = 0; row < size_; ++row) {
        for (int col = 0; col < size_; ++col) {
            if (at(row, col) == 0) {
                empty.push_back({row, col});
            }
        }
    }
    return empty;
}

bool Board::canMove() const {
    for (int row = 0; row < size_; ++row) {
        for (int col = 0; col < size_; ++col) {
            const int value = at(row, col);
            if (value == 0) {
                return true;
            }
            if (col + 1 < size_ && at(row, col + 1) == value) {
                return true;
            }
            if (row + 1 < size_ && at(row + 1, col) == value) {
                return true;
            }
        }
    }
    return false;
}

int Board::maxTile() const {
    return *std::max_element(cells_.begin(), cells_.end());
}

bool Board::operator==(const Board& other) const noexcept {
    return size_ == other.size_ && cells_ == other.cells_;
}

bool Board::operator!=(const Board& other) const noexcept {
    return !(*this == other);
}

}  // namespace twenty48
