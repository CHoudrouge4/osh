#pragma once
#include "labs.h"
#include <vector>
#include <tuple>
#include <random>
#include <iostream>

using statItem = std::tuple<int64_t, long, double>;

long long get_time_mcs();

struct Solver {
	Solver(const Labs);

	virtual Solver* clone() const = 0;

	// Optimal f
	double get_opt();
	// Optimal vector
	Bvec get_opt_vec();

	// Accepts time in ms, returns true if it found the optimum (or
	// false for "timed out").
	virtual bool run(long long timeout) = 0;
	void reset();

	void sbm(Bvec&);
	void sbm(Bvec&, int);
	void uni_crossover(Bvec&, const Bvec&, const Bvec&);

	// Re-initialise stats variables
	void record_begin();
	// Record current stats
	void record_current();
	// Statistics. Each element is (time,iteration,value)
	std::vector<statItem> stats;
	// Total algorithm running time in ms.
	long long running_time;

protected:
	Labs labs; // labs instance we work on
	double opt; // optimal value (F)
	Bvec opt_vec; // optimal vector

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> uni_dis_N; // [0,labs.N]
	std::uniform_int_distribution<int> fair_coin;
	std::uniform_real_distribution<double> uni_dis_one; // (0,1)

	// The time last start was began on.
	double last_run_start;
	// Getting current running time
	long long get_running_time_mcs();
	long long get_running_time_ms();
};
