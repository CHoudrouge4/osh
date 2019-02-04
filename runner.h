#pragma once
#include "solver.h"
#include "method.h"
#include <vector>

struct Runner {
	Runner();

	// todo add threads
	void execute(std::vector<Solver*> s, int sample_size, int timeout);

	// hits number and ratio
	int hits_n;
	double hits_ratio;
	// global vector of stats
	std::vector<std::vector<statItem>> stats;
	// average running time
	int average_t;
	// execution durations
	std::vector<long long> exec_durations;
};
