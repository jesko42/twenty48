#pragma once

#include <random>

namespace twenty48 {

/// Source of randomness, injected into the game so that tests can script it (SPEC R4).
class Rng {
public:
    Rng() = default;
    Rng(const Rng&) = delete;
    Rng& operator=(const Rng&) = delete;
    Rng(Rng&&) = delete;
    Rng& operator=(Rng&&) = delete;
    virtual ~Rng() = default;

    /// Returns a uniformly distributed integer in [0, bound). bound must be >= 1.
    [[nodiscard]] virtual int below(int bound) = 0;
};

/// Mersenne-Twister based implementation, deterministic for a given seed.
class StdRng final : public Rng {
public:
    explicit StdRng(unsigned int seed);
    [[nodiscard]] int below(int bound) override;

private:
    std::mt19937 engine_;
};

}  // namespace twenty48
