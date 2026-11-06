#include "twenty48/rng.hpp"

#include <stdexcept>

namespace twenty48 {

StdRng::StdRng(unsigned int seed) : engine_(seed) {}

int StdRng::below(int bound) {
    if (bound < 1) {
        throw std::invalid_argument("bound must be at least 1");
    }
    std::uniform_int_distribution<int> dist(0, bound - 1);
    return dist(engine_);
}

}  // namespace twenty48
