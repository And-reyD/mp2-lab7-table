#pragma once
#include <cstdlib>
#include <random>

class Randomex {
private:
	Randomex() { }
public:
	static void Init() {
		srand((unsigned int)time(nullptr));
	}
	static int RandInt(int rangeMin, int rangeMax) {
		return (int)((double)rand() / (RAND_MAX + 1) * ((rangeMax + 1) - rangeMin) + rangeMin);
	}
};