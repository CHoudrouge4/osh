#pragma once
#include "labs.h"
#include <vector>
#include <tuple>
#include <random>
#include <iostream>

using statItem = std::tuple<long long, int, double>;

class Solver {
public:
	Solver(const Labs&);

	Bvec getOptimal();

	virtual void run(int iterNum) = 0; // better: time limit in ms
	virtual void reset() = 0;

	void sbm(Bvec&);
	void sbm(Bvec&, int);

	// Re-initialise stats variables
	void recordBegin();
	// Record current stats
	void recordCurrent(int iterNum);
	// Obtain the statistics
	std::vector<statItem> getStats();

protected:
	Labs labs;

	double opt;
	Bvec opt_val;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> uni_dis;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;

	// The time last start was began on.
	double last_run_start;
	// Statistics. Each element is (time,iteration,value)
	std::vector<statItem> stats;
};
