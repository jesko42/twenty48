// Examples taken verbatim from SPEC.md, section R2/R3 ("Schieben und Verschmelzen").
#include <string>

#include "doctest/doctest.h"

#include "twenty48/row.hpp"

using twenty48::Row;
using twenty48::slideLeft;

namespace {
struct Example {
    std::string spec;  // reference into SPEC.md
    Row input;
    Row expected;
    int score;
    bool changed;
};
}  // namespace

TEST_CASE("slideLeft follows the SPEC examples") {
    const Example examples[] = {
        {"R2 slide into gaps", {0, 0, 0, 2}, {2, 0, 0, 0}, 0, true},
        {"R2 keep order", {0, 2, 0, 4}, {2, 4, 0, 0}, 0, true},
        {"R3 simple merge", {2, 2, 0, 0}, {4, 0, 0, 0}, 4, true},
        {"R3 merge across gap", {2, 0, 2, 4}, {4, 4, 0, 0}, 4, true},
        {"R3 merge once per tile", {2, 2, 2, 2}, {4, 4, 0, 0}, 8, true},
        {"R3 no chain merge", {2, 2, 4, 0}, {4, 4, 0, 0}, 4, true},
        {"R3 merge nearest the wall first", {2, 2, 2, 0}, {4, 2, 0, 0}, 4, true},
        {"R3 two pairs", {2, 2, 4, 4}, {4, 8, 0, 0}, 12, true},
        {"R3 merge after sliding", {4, 2, 2, 0}, {4, 4, 0, 0}, 4, true},
        {"R6 nothing to do", {2, 4, 2, 4}, {2, 4, 2, 4}, 0, false},
        {"R6 already left", {2, 4, 0, 0}, {2, 4, 0, 0}, 0, false},
        {"R6 empty row", {0, 0, 0, 0}, {0, 0, 0, 0}, 0, false},
    };

    for (const Example& ex : examples) {
        CAPTURE(ex.spec);
        const auto result = slideLeft(ex.input);
        CHECK(result.row == ex.expected);
        CHECK(result.score == ex.score);
        CHECK(result.changed == ex.changed);
    }
}

TEST_CASE("slideLeft works for rows of any length") {
    CHECK(slideLeft(Row{2, 2, 2, 2, 2}).row == Row{4, 4, 2, 0, 0});
    CHECK(slideLeft(Row{8, 8}).row == Row{16, 0});
}
