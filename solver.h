#pragma once
#include "labs.h"
#include <vector>
#include <random>
#include <iostream>

class Solver {
public:
	explicit Solver(const Labs&);

	Bvec getOptimal();

	virtual void run(int iterNum) = 0; // better: time limit in ms
	virtual void reset() = 0;

	void sbm(Bvec&);
	void sbm(Bvec&, int);

protected:
	Labs labs;

	double opt;
	Bvec opt_val;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> uni_dis;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
