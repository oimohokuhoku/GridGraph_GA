#include "Random.hpp"

void Random::init() {
    std::random_device rnd;
    generator = std::mt19937(rnd());
}

void Random::init(int seed) {
    generator = std::mt19937(seed);
}

int Random::randomInt(int min, int max) {
    int range = max - min;
    return (generator() % range) + min;
}

int Random::randomInt(int max) {
    return generator() % max;
}

double Random::randomDouble() {
    return (double)generator() / 0xffffffff;
}

double Random::randomDouble(double min, double max) {
    double r = randomDouble();
    return (r * (max - min)) + min;
}

std::mt19937 Random::generator;