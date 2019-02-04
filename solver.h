#pragma once
#include "labs.h"
#include <vector>
#include <tuple>
#include <random>
#include <iostream>

using statItem = std::tuple<int64_t, long, double>;

struct Solver {
	Solver(Labs);

	double get_opt();
	Bvec getOptimal();

	// Accepts time in ms, returns true if it found the optimum (or
	// false for "timed out").
	virtual bool run(long long timeout) = 0;
	void reset();

	void sbm(Bvec&);
	void sbm(Bvec&, int);
	void uni_crossover(Bvec&, const Bvec&);

	// Re-initialise stats variables
	void recordBegin();
	// Record current stats
	void recordCurrent();
	// Statistics. Each element is (time,iteration,value)
	std::vector<statItem> stats;
	// Running time in ms.
	long long running_time;

protected:
	Labs labs;

	double opt;
	Bvec opt_val;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> uni_dis_N; // [0,labs.N]
	std::uniform_int_distribution<int> fair_coin;
	std::uniform_real_distribution<double> uni_dis_one; // (0,1)

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;

	// The time last start was began on.
	double last_run_start;
	// Getting current runningtime
	long long getRunningTimeMcs();
	long long getRunningTimeMs();
};
