#pragma once
#include "bvec.h"
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>

// TODO add memoization
class Labs {
	std::unordered_map<Bvec,double> memo;
public:
	const int N;
	const Bvec optVec;
	const int optE;
	const double optF;

	Labs(int);
	int E(const Bvec&);
	double F(const Bvec&);
};
