#ifndef RANDOM_NUMBERS_H
#define RANDOM_NUMBERS_H
#include <random>

// Function to generate a random integer in the range [min, max]
inline int getRandomInt(int min, int max) {
    // Create a random number engine
    std::random_device rd;
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

    // Create a distribution for the desired range
    std::uniform_int_distribution<> dis(min, max);

    // Generate and return a random number
    return dis(gen);
}

#endif