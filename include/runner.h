#pragma once
#include "solver.h"
#include "method.h"
#include <vector>

struct Runner {
	Runner();

	// solvers is a list of <solver,timeout>
	void execute(std::vector<std::pair<Solver*,long long>> solvers
				  , int threads_n
				  , int sample_size
				  , std::string dir_pattern);
};
