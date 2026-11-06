// Terminal front end for twenty48. All game rules live in twenty48::core; this file only
// parses arguments, renders the board and translates key presses into moves.

#include <unistd.h>

#include <cctype>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "twenty48/game.hpp"

namespace {

struct Options {
    unsigned int seed = 0;
    bool seeded = false;
    std::string moves;  // scripted moves, e.g. "lrud"; empty means interactive
    bool help = false;
};

void printUsage(std::ostream& out) {
    out << "Usage: twenty48 [--seed N] [--moves LRUD...]\n"
           "  --seed N      deterministic tile placement\n"
           "  --moves S     play the given moves (l/r/u/d) and exit\n"
           "Keys: a=left d=right w=up s=down u=undo q=quit (confirm each key with Enter)\n";
}

std::optional<Options> parseArgs(int argc, char** argv) {
    Options opts;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") {
            opts.help = true;
        } else if (arg == "--seed" && i + 1 < argc) {
            opts.seed = static_cast<unsigned int>(std::strtoul(argv[++i], nullptr, 10));
            opts.seeded = true;
        } else if (arg == "--moves" && i + 1 < argc) {
            opts.moves = argv[++i];
        } else {
            std::cerr << "unknown argument: " << arg << "\n";
            return std::nullopt;
        }
    }
    return opts;
}

std::optional<twenty48::Direction> toDirection(char key) {
    switch (std::tolower(static_cast<unsigned char>(key))) {
        case 'a':
        case 'l':
            return twenty48::Direction::Left;
        case 'd':
        case 'r':
            return twenty48::Direction::Right;
        case 'w':
        case 'u':
            return twenty48::Direction::Up;
        case 's':
            return twenty48::Direction::Down;
        default:
            return std::nullopt;
    }
}

void render(const twenty48::Game& game, std::ostream& out) {
    const twenty48::Board& board = game.board();
    const int n = board.size();
    const std::string separator = [n] {
        std::string s = "+";
        for (int i = 0; i < n; ++i) {
            s += "------+";
        }
        return s;
    }();

    if (isatty(STDOUT_FILENO) != 0) {
        out << "\x1b[2J\x1b[H";  // clear screen, cursor home
    }
    out << "twenty48    Score: " << game.score() << "\n\n";
    for (int row = 0; row < n; ++row) {
        out << separator << "\n|";
        for (int col = 0; col < n; ++col) {
            const int value = board.at(row, col);
            if (value == 0) {
                out << "      |";
            } else {
                out << std::setw(5) << value << " |";
            }
        }
        out << "\n";
    }
    out << separator << "\n";
    if (game.won()) {
        out << "\nYou reached " << twenty48::Game::kWinTile << ". You win!\n";
    }
    if (game.over()) {
        out << "\nNo moves left. Game over.\n";
    }
}

}  // namespace

int main(int argc, char** argv) {
    const std::optional<Options> opts = parseArgs(argc, argv);
    if (!opts) {
        printUsage(std::cerr);
        return 2;
    }
    if (opts->help) {
        printUsage(std::cout);
        return 0;
    }

    const unsigned int seed =
        opts->seeded ? opts->seed
                     : static_cast<unsigned int>(
                           std::chrono::steady_clock::now().time_since_epoch().count());
    twenty48::StdRng rng(seed);
    twenty48::Game game(rng);
    game.start();

    if (!opts->moves.empty()) {
        for (const char key : opts->moves) {
            if (const auto dir = toDirection(key)) {
                game.move(*dir);
            }
        }
        render(game, std::cout);
        return 0;
    }

    std::string line;
    while (true) {
        render(game, std::cout);
        if (game.over()) {
            return 0;
        }
        std::cout << "\nmove [a/d/w/s, u=undo, q=quit] > " << std::flush;
        if (!std::getline(std::cin, line)) {
            return 0;
        }
        std::istringstream in(line);
        char key = '\0';
        if (!(in >> key)) {
            continue;
        }
        if (key == 'q') {
            return 0;
        }
        if (key == 'u') {
            if (!game.undo()) {
                std::cout << "Nothing to undo.\n";
            }
            continue;
        }
        if (const auto dir = toDirection(key)) {
            game.move(*dir);
        }
    }
}
