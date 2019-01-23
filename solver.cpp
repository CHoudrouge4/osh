#include "solver.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>

std::random_device Solver::rd;
std::mt19937 Solver::gen(rd());

Solver::Solver (const Labs& l): labs(l) {
	S = ::bvec(l.N, false);
	bin_dis = std::binomial_distribution<int>(l.N, 1/((double)l.N));
	dis = std::uniform_int_distribution<int>(1, l.N);
}

void Solver::sbm(bvec& x, int l) {
	for(int i = 0; i < l; ++i) {
		int index = dis(gen);
		x[index] = x[index] xor 1;
	}
}

void Solver::sbm(bvec& x) { sbm(x, bin_dis(gen)); }
