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

Solver::Solver (Labs l): labs(l), opt_val(l.N) {
	bin_dis = std::binomial_distribution<int>(l.N-1, 1/((double)l.N));
	uni_dis_N = std::uniform_int_distribution<int>(0, l.N-1);
	uni_dis_one = std::uniform_real_distribution<double>(0, 1);
	fair_coin = std::uniform_int_distribution<int> (0, 1);
}

void Solver::reset() {
	labs.callsNum = 0;
	opt_val.clear();
	opt = labs.F(opt_val);
	stats.clear();
	running_time = 0;
}

void Solver::sbm(Bvec& x, int l) {
	for(int i = 0; i < l; ++i) x.flipBit(uni_dis_N(gen));
}

void Solver::sbm(Bvec& x) {
	int l = bin_dis(gen);
	sbm(x, l);
}

void Solver::uni_crossover(Bvec& target, const Bvec& other) {
	for (int i = 0; i < labs.N; i++) {
		int rnd = fair_coin(gen);
		if (rnd % 2 == 0) target.set(i, other.get(i));
	}
}

Bvec Solver::getOptimal() { return opt_val; }
double Solver::get_opt() { return opt; }

long long getTimeMcs() {
	uint64_t us =
		std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return us;
}

long long Solver::getRunningTimeMcs() {
	return getTimeMcs() - last_run_start;
}

long long Solver::getRunningTimeMs() {
	long long x = getRunningTimeMcs();
	return x / 1000;
}

void Solver::recordBegin() {
	stats.clear();
	last_run_start = getTimeMcs();
}

void Solver::recordCurrent() {
	long long cur_time = getRunningTimeMcs();
	stats.push_back(std::make_tuple(cur_time, labs.callsNum, opt));
}
