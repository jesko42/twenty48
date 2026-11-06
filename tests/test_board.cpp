#include <stdexcept>

#include "doctest/doctest.h"
#include "helpers.hpp"

#include "twenty48/board.hpp"

using twenty48::Board;
using twenty48::Direction;
using twenty48::test::Grid;
using twenty48::test::makeBoard;
using twenty48::test::toGrid;

TEST_CASE("R1: a new board is empty and square") {
    const Board board;
    CHECK(board.size() == 4);
    CHECK(board.emptyCells().size() == 16);
    CHECK(board.maxTile() == 0);
}

TEST_CASE("R1: board size is configurable but at least 2x2") {
    CHECK(Board(3).emptyCells().size() == 9);
    CHECK_THROWS_AS(Board(1), std::invalid_argument);
}

TEST_CASE("R1: coordinates outside the board are rejected") {
    Board board;
    CHECK_THROWS_AS(static_cast<void>(board.at(4, 0)), std::out_of_range);
    CHECK_THROWS_AS(static_cast<void>(board.at(0, -1)), std::out_of_range);
    CHECK_THROWS_AS(board.set(0, 4, 2), std::out_of_range);
}

TEST_CASE("R2/R3: a move slides every line towards the chosen wall") {
    Board board = makeBoard({
        {2, 0, 2, 0},
        {0, 4, 0, 4},
        {2, 2, 2, 2},
        {0, 0, 0, 8},
    });

    SUBCASE("left") {
        const auto result = board.move(Direction::Left);
        CHECK(toGrid(board) == Grid{{4, 0, 0, 0}, {8, 0, 0, 0}, {4, 4, 0, 0}, {8, 0, 0, 0}});
        CHECK(result.score == 4 + 8 + 8);
        CHECK(result.changed);
    }
    SUBCASE("right") {
        const auto result = board.move(Direction::Right);
        CHECK(toGrid(board) == Grid{{0, 0, 0, 4}, {0, 0, 0, 8}, {0, 0, 4, 4}, {0, 0, 0, 8}});
        CHECK(result.score == 20);
        CHECK(result.changed);
    }
    SUBCASE("up") {
        const auto result = board.move(Direction::Up);
        CHECK(toGrid(board) == Grid{{4, 4, 4, 4}, {0, 2, 0, 2}, {0, 0, 0, 8}, {0, 0, 0, 0}});
        CHECK(result.score == 4 + 4);
        CHECK(result.changed);
    }
    SUBCASE("down") {
        const auto result = board.move(Direction::Down);
        CHECK(toGrid(board) == Grid{{0, 0, 0, 0}, {0, 0, 0, 4}, {0, 4, 0, 2}, {4, 2, 4, 8}});
        CHECK(result.score == 4 + 4);
        CHECK(result.changed);
    }
}

TEST_CASE("R6: a move that changes nothing reports changed == false") {
    Board board = makeBoard({
        {2, 4, 0, 0},
        {8, 2, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0},
    });
    const Board before = board;
    const auto result = board.move(Direction::Left);
    CHECK_FALSE(result.changed);
    CHECK(result.score == 0);
    CHECK(board == before);
}

TEST_CASE("R9: canMove is false only when the board is full and nothing merges") {
    CHECK(Board().canMove());
    CHECK(makeBoard({{2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 0}}).canMove());
    CHECK(makeBoard({{2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 4}}).canMove());
    CHECK(makeBoard({{2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 4}}).canMove());
    CHECK_FALSE(makeBoard({{2, 4, 2, 4}, {4, 2, 4, 2}, {2, 4, 2, 4}, {4, 2, 4, 2}}).canMove());
}

TEST_CASE("maxTile returns the largest tile") {
    CHECK(makeBoard({{2, 4, 0}, {0, 64, 0}, {0, 0, 8}}).maxTile() == 64);
}
