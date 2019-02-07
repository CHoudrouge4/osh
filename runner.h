#pragma once
#include "solver.h"
#include "method.h"
#include <vector>

struct Runner {
	Runner();

	// todo add threads
	void execute(std::vector<Solver*> s, int sample_size, int timeout, std::string dir);

	// hits number and ratio
	int hits_n;
	double hits_ratio;
	// average running time
	int average_t;
	// execution durations
	std::vector<long long> exec_durations;
};
