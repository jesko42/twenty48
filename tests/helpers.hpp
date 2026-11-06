#pragma once

#include <initializer_list>
#include <stdexcept>
#include <string>
#include <vector>

#include "twenty48/board.hpp"
#include "twenty48/rng.hpp"

namespace twenty48::test {

using Grid = std::vector<std::vector<int>>;

/// Builds a board from rows, e.g. makeBoard({{2, 2, 0, 0}, {0, 0, 0, 0}, ...}).
inline Board makeBoard(const Grid& rows) {
    Board board(static_cast<int>(rows.size()));
    for (int r = 0; r < board.size(); ++r) {
        for (int c = 0; c < board.size(); ++c) {
            board.set(r, c, rows[static_cast<std::size_t>(r)][static_cast<std::size_t>(c)]);
        }
    }
    return board;
}

inline Grid toGrid(const Board& board) {
    Grid rows;
    for (int r = 0; r < board.size(); ++r) {
        std::vector<int> row;
        for (int c = 0; c < board.size(); ++c) {
            row.push_back(board.at(r, c));
        }
        rows.push_back(row);
    }
    return rows;
}

/// Returns pre-defined values in order; throws if the game asks for more than scripted.
class ScriptedRng final : public Rng {
public:
    explicit ScriptedRng(std::vector<int> values) : values_(std::move(values)) {}

    int below(int bound) override {
        if (next_ >= values_.size()) {
            throw std::runtime_error("ScriptedRng exhausted after " + std::to_string(next_) +
                                     " values");
        }
        const int value = values_[next_++];
        if (value < 0 || value >= bound) {
            throw std::runtime_error("scripted value " + std::to_string(value) +
                                     " is not below bound " + std::to_string(bound));
        }
        return value;
    }

    [[nodiscard]] std::size_t calls() const noexcept { return next_; }

private:
    std::vector<int> values_;
    std::size_t next_ = 0;
};

}  // namespace twenty48::test
