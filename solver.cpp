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

Solver::Solver (Labs l): labs(l), opt_vec(l.N) {
	bin_dis = std::binomial_distribution<int>(l.N-1, 1/((double)l.N));
	uni_dis_N = std::uniform_int_distribution<int>(0, l.N-1);
	uni_dis_one = std::uniform_real_distribution<double>(0, 1);
	fair_coin = std::uniform_int_distribution<int> (0, 1);
}

void Solver::reset() {
	labs.calls_num = 0;
	opt_vec.clear();
	opt = labs.F(opt_vec);
	stats.clear();
	running_time = 0;
}

void Solver::sbm(Bvec& x, int l) {
	for(int i = 0; i < l; ++i) x.flip_bit(uni_dis_N(gen));
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

double Solver::get_opt() { return opt; }
Bvec Solver::get_opt_vec() { return opt_vec; }

long long get_time_mcs() {
	uint64_t us =
		std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	return us;
}

long long Solver::get_running_time_mcs() {
	return get_time_mcs() - last_run_start;
}

long long Solver::get_running_time_ms() {
	long long x = get_running_time_mcs();
	return x / 1000;
}

void Solver::record_begin() {
	stats.clear();
	last_run_start = get_time_mcs();
}

void Solver::record_current() {
	long long cur_time = get_running_time_mcs();
	stats.push_back(std::make_tuple(cur_time, labs.calls_num, opt));
}
