#include "solver.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <chrono>
#include <ratio>
#include <thread>

std::random_device Solver::rd;
std::mt19937 Solver::gen(rd());

Solver::Solver (const Labs& l): labs(l), opt_val(l.N) {
	bin_dis = std::binomial_distribution<int>(l.N-1, 1/((double)l.N));
	uni_dis_N = std::uniform_int_distribution<int>(0, l.N-1);
	uni_dis_one = std::uniform_real_distribution<double>(0, 1);
}

void Solver::sbm(Bvec& x, int l) {
	for(int i = 0; i < l; ++i) x.flipBit(uni_dis_N(gen));
}

void Solver::sbm(Bvec& x) {
	int l = bin_dis(gen);
	sbm(x, l);
}

void Solver::uni_crossover(Bvec& target, const Bvec& other) {
	for (int i = 0; i < labs.N; i++)
		if (rand() % 2 == 0) target.set(i, other.get(i));
}

Bvec Solver::getOptimal() { return opt_val; }

long long getTimeMs() {
	uint64_t us =
		std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return us;
}

void Solver::recordBegin() {
	stats.clear();
	last_run_start = getTimeMs();
}

void Solver::recordCurrent(int iterNum) {
	long long cur_time = getTimeMs() - last_run_start;
	stats.push_back(std::make_tuple(cur_time, iterNum, opt));
}

std::vector<statItem>Solver::getStats() { return stats; }

void Solver::print_sequence() const {
	std::cout << "Optimal Sequence is: " << opt_val << '\n';
}
