#pragma once
#include "bvec.h"
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>

class Labs {
	static bool enable_memo;
	std::unordered_map<Bvec,double> memo;
public:
	int N;
	const Bvec opt_vec;
	const int optE;
	const double optF;
	long calls_num; // number of calls to E/F

	Labs(int);

	int E(const Bvec&);
	double F(const Bvec&);

	static int corr(int n, int k, const Bvec &S);
	static int energy(int n, const Bvec &S);
	static double merit (int n, const Bvec &S);
};
