cmake_minimum_required(VERSION 3.25)

project(twenty48
    VERSION 0.1.0
    DESCRIPTION "2048 in the terminal, built spec-driven with CMake and C++17"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TWENTY48_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" ON)
option(TWENTY48_SANITIZERS "Build with AddressSanitizer and UndefinedBehaviorSanitizer" OFF)
option(TWENTY48_COVERAGE "Build with coverage instrumentation (GCC/Clang)" OFF)
option(TWENTY48_BUILD_TESTS "Build the unit tests" ON)

include(cmake/ProjectOptions.cmake)

# Core library: pure game logic, no I/O, no randomness of its own (RNG is injected).
add_library(twenty48_core
    src/row.cpp
    src/board.cpp
    src/rng.cpp
    src/game.cpp)
add_library(twenty48::core ALIAS twenty48_core)
target_include_directories(twenty48_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
twenty48_apply_options(twenty48_core)

# Terminal front end.
add_executable(twenty48 src/main.cpp)
target_link_libraries(twenty48 PRIVATE twenty48::core)
twenty48_apply_options(twenty48)

if(TWENTY48_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
