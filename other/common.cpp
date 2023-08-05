#include <cstdio>
#include <Random>
#include "common.hpp"

std::mt19937 mt;

void initRand() {
	std::random_device rnd;
	mt = std::mt19937(rnd());
}

/// @brief generate random integer (min <= n < max)
int randInt(int min, int max) {
	return (mt() % (max - min)) + min;
}

/// @brief generate random real number (0 <= n < 1)
double randDouble() {
	return (double)mt() / 0xffffffff;
}

