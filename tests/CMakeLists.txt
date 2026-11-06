add_executable(twenty48_tests
    test_main.cpp
    test_row.cpp
    test_board.cpp
    test_game.cpp)
# SYSTEM keeps the third-party header out of our -Werror scope.
target_include_directories(twenty48_tests SYSTEM PRIVATE ${CMAKE_SOURCE_DIR}/third_party)
target_link_libraries(twenty48_tests PRIVATE twenty48::core)
twenty48_apply_options(twenty48_tests)

add_test(NAME unit_tests COMMAND twenty48_tests)

# Acceptance smoke test: the CLI plays a scripted game deterministically and exits cleanly.
add_test(NAME cli_smoke COMMAND twenty48 --seed 42 --moves lrudlrud)
set_tests_properties(cli_smoke PROPERTIES PASS_REGULAR_EXPRESSION "Score:")
