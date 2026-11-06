#pragma once

#include <vector>

#include "twenty48/board.hpp"
#include "twenty48/rng.hpp"

namespace twenty48 {

/// Game rules on top of Board: spawning, scoring, win and game-over detection.
class Game {
public:
    static constexpr int kWinTile = 2048;  ///< SPEC R8

    explicit Game(Rng& rng, int size = Board::kDefaultSize);

    /// Continues from a given position, e.g. for tests or a saved game.
    Game(Rng& rng, Board board, int score = 0);

    /// Places the two starting tiles (SPEC R5).
    void start();

    /// Applies a move. Returns false and changes nothing if the move would not alter the board
    /// (SPEC R6). On success the score grows and a new tile spawns (SPEC R4, R7).
    bool move(Direction dir);

    /// Reverts the last effective move, including its spawned tile (SPEC R11).
    /// Returns false and changes nothing if there is no move to undo.
    bool undo();
    [[nodiscard]] bool canUndo() const noexcept { return !history_.empty(); }

    [[nodiscard]] const Board& board() const noexcept { return board_; }
    [[nodiscard]] int score() const noexcept { return score_; }
    [[nodiscard]] bool won() const;   ///< SPEC R8
    [[nodiscard]] bool over() const;  ///< SPEC R9

private:
    struct Snapshot {
        Board board;
        int score = 0;
    };

    void spawnTile();

    Rng* rng_;  // non-owning; the RNG outlives the game
    Board board_;
    int score_ = 0;
    std::vector<Snapshot> history_;  // one entry per effective move (R11.4)
};

}  // namespace twenty48
