#pragma once
#include "bvec.h"
#include <vector>
#include <random>
#include <iostream>

// TODO add memoization
struct Labs {
	const int N;
	const Bvec optVec;
	const int optE;
	const double optF;

	Labs(int);
	int E(const Bvec&);
	double F(const Bvec&);
};
