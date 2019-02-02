#pragma once
#include "bvec.h"
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>

class Labs {
	std::unordered_map<Bvec,double> memo;
public:
	const int N;
	const Bvec optVec;
	const int optE;
	const double optF;
	long callsNum; // number of calls to E/F

	Labs(int);
	int E(const Bvec&);
	double F(const Bvec&);
};
