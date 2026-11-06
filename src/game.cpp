#include "twenty48/game.hpp"

#include <utility>

namespace twenty48 {

namespace {
constexpr int kFourTileOneIn = 10;  // a spawned tile is a 4 with probability 1/10 (R4)
}

Game::Game(Rng& rng, int size) : rng_(&rng), board_(size) {}

Game::Game(Rng& rng, Board board, int score)
    : rng_(&rng), board_(std::move(board)), score_(score) {}

void Game::start() {
    spawnTile();
    spawnTile();
}

bool Game::move(Direction dir) {
    Snapshot before{board_, score_};
    const MoveResult result = board_.move(dir);
    if (!result.changed) {
        return false;  // board_ is untouched when nothing changed (R6.2)
    }
    history_.push_back(std::move(before));
    score_ += result.score;
    spawnTile();
    return true;
}

bool Game::undo() {
    if (history_.empty()) {
        return false;
    }
    board_ = std::move(history_.back().board);
    score_ = history_.back().score;
    history_.pop_back();
    return true;
}

bool Game::won() const {
    return board_.maxTile() >= kWinTile;
}

bool Game::over() const {
    return !board_.canMove();
}

void Game::spawnTile() {
    const std::vector<Cell> empty = board_.emptyCells();
    if (empty.empty()) {
        return;
    }
    // Order matters for scripted RNGs in tests: first the cell, then the value (R4).
    const Cell cell = empty[static_cast<std::size_t>(rng_->below(static_cast<int>(empty.size())))];
    const int value = rng_->below(kFourTileOneIn) == 0 ? 4 : 2;
    board_.set(cell.row, cell.col, value);
}

}  // namespace twenty48
