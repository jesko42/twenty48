#include <vector>

#include "doctest/doctest.h"
#include "helpers.hpp"

#include "twenty48/game.hpp"

using twenty48::Board;
using twenty48::Direction;
using twenty48::Game;
using twenty48::test::Grid;
using twenty48::test::makeBoard;
using twenty48::test::ScriptedRng;
using twenty48::test::toGrid;

TEST_CASE("R5: start places exactly two tiles") {
    // Scripted: cell 0 of 16 empties -> (0,0), value roll 3 -> 2;
    //           cell 14 of 15 empties -> (3,3), value roll 0 -> 4.
    ScriptedRng rng({0, 3, 14, 0});
    Game game(rng);

    game.start();

    CHECK(game.board().emptyCells().size() == 14);
    CHECK(game.board().at(0, 0) == 2);
    CHECK(game.board().at(3, 3) == 4);
    CHECK(game.score() == 0);
    CHECK(rng.calls() == 4);
}

TEST_CASE("R4: a spawned tile is a 4 for value roll 0 and a 2 otherwise") {
    for (int roll = 0; roll < 10; ++roll) {
        CAPTURE(roll);
        ScriptedRng rng({0, roll});  // exactly one empty cell after the move
        Game game(rng, makeBoard({{2, 2}, {4, 8}}));

        REQUIRE(game.move(Direction::Left));

        CHECK(game.board().at(0, 0) == 4);
        CHECK(game.board().at(0, 1) == (roll == 0 ? 4 : 2));
    }
}

TEST_CASE("R4: the new tile goes into the scripted empty cell") {
    ScriptedRng rng({2, 5});  // third empty cell in row-major order
    Game game(rng, makeBoard({{2, 0, 0}, {2, 0, 0}, {0, 0, 0}}));

    REQUIRE(game.move(Direction::Up));

    // After Up: (0,0)=4. Empties in row-major order: (0,1) (0,2) (1,0) ... -> index 2 is (1,0).
    CHECK(toGrid(game.board()) == Grid{{4, 0, 0}, {2, 0, 0}, {0, 0, 0}});
}

TEST_CASE("R6/R7: a successful move adds the merge score and spawns exactly one tile") {
    ScriptedRng rng({0, 3});
    Game game(rng, makeBoard({{2, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}));

    const bool moved = game.move(Direction::Left);

    CHECK(moved);
    CHECK(game.score() == 4);
    CHECK(game.board().at(0, 0) == 4);
    CHECK(game.board().emptyCells().size() == 14);
    CHECK(rng.calls() == 2);
}

TEST_CASE("R6: a move that changes nothing spawns no tile and does not touch the RNG") {
    ScriptedRng rng({});
    Game game(rng, makeBoard({{2, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}));
    const Board before = game.board();

    const bool moved = game.move(Direction::Left);

    CHECK_FALSE(moved);
    CHECK(game.board() == before);
    CHECK(game.score() == 0);
    CHECK(rng.calls() == 0);
}

TEST_CASE("R7: the score accumulates over several moves") {
    ScriptedRng rng({0, 3, 0, 3});
    Game game(rng, makeBoard({{2, 2, 4, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}));

    REQUIRE(game.move(Direction::Left));  // 4 + 8 = 12, new tile 2 at (0,2)
    CHECK(game.score() == 12);
    REQUIRE(game.move(Direction::Right));  // row 0: 4 8 2 -> slides right, no merge, +0
    CHECK(game.score() == 12);
}

TEST_CASE("R8: the game is won as soon as a 2048 tile exists") {
    ScriptedRng rng({0, 3});
    Game game(rng, makeBoard({{1024, 1024}, {2, 4}}));
    CHECK_FALSE(game.won());

    REQUIRE(game.move(Direction::Left));

    CHECK(game.won());
    CHECK(game.score() == 2048);
}

TEST_CASE("R9: the game is over when no move is possible") {
    ScriptedRng rng({});
    CHECK(Game(rng, makeBoard({{2, 4}, {4, 2}})).over());
    CHECK_FALSE(Game(rng, makeBoard({{2, 4}, {4, 4}})).over());
    CHECK_FALSE(Game(rng, makeBoard({{2, 4}, {4, 0}})).over());
}

TEST_CASE("R4: spawning on a full board does nothing and does not touch the RNG") {
    ScriptedRng rng({});
    Game game(rng, makeBoard({{2, 4}, {4, 2}}));

    game.start();  // no empty cell -> nothing to spawn, must not throw

    CHECK(toGrid(game.board()) == Grid{{2, 4}, {4, 2}});
    CHECK(rng.calls() == 0);
}

TEST_CASE("R11.1: undo restores board and score from before the last move") {
    ScriptedRng rng({0, 3});
    Game game(rng, makeBoard({{2, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}), 10);
    const Board before = game.board();
    REQUIRE(game.move(Direction::Left));
    REQUIRE(game.score() == 14);
    REQUIRE(game.canUndo());

    CHECK(game.undo());

    CHECK(game.board() == before);
    CHECK(game.score() == 10);
    CHECK_FALSE(game.canUndo());
}

TEST_CASE("R11.2: several moves are undone in reverse order down to the start") {
    ScriptedRng rng({0, 3, 0, 3});
    Game game(rng, makeBoard({{2, 2, 4, 4}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}));
    const Board start = game.board();
    REQUIRE(game.move(Direction::Left));
    const Board afterFirst = game.board();
    const int scoreAfterFirst = game.score();
    REQUIRE(game.move(Direction::Right));

    CHECK(game.undo());
    CHECK(game.board() == afterFirst);
    CHECK(game.score() == scoreAfterFirst);

    CHECK(game.undo());
    CHECK(game.board() == start);
    CHECK(game.score() == 0);

    CHECK_FALSE(game.undo());
}

TEST_CASE("R11.3: undo right after the start changes nothing and reports false") {
    ScriptedRng rng({0, 3, 0, 3});
    Game game(rng);
    game.start();
    const Board board = game.board();

    CHECK_FALSE(game.canUndo());
    CHECK_FALSE(game.undo());
    CHECK(game.board() == board);
    CHECK(game.score() == 0);
}

TEST_CASE("R11.4: an ineffective move leaves nothing to undo") {
    ScriptedRng rng({});
    Game game(rng, makeBoard({{2, 4, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}));

    CHECK_FALSE(game.move(Direction::Left));

    CHECK_FALSE(game.canUndo());
    CHECK_FALSE(game.undo());
}
