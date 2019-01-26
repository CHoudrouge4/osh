#include "solver.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>

std::random_device Solver::rd;
std::mt19937 Solver::gen(rd());

Solver::Solver (const Labs& l): labs(l), opt_val(l.N) {
	bin_dis = std::binomial_distribution<int>(l.N-1, 1/((double)l.N));
	uni_dis = std::uniform_int_distribution<int>(0, l.N-1);
}

void Solver::sbm(Bvec& x, int l) {
	for(int i = 0; i < l; ++i) x.flipBit(uni_dis(gen));
}

void Solver::sbm(Bvec& x) {
	int l = bin_dis(gen);
	sbm(x, l);
}

Bvec Solver::getOptimal() { return opt_val; }
