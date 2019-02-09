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

Solver::Solver (const Labs l): labs(l), opt_vec(l.N) {
	bin_dis = std::binomial_distribution<int>(l.N-1, 1/((double)l.N));
	uni_dis_N = std::uniform_int_distribution<int>(0, l.N-1);
	uni_dis_one = std::uniform_real_distribution<double>(0, 1);
	fair_coin = std::uniform_int_distribution<int> (0, 1);
	tau = std::vector<std::vector<int>> (l.N - 1, std::vector<int>(l.N - 1));
	c_s = std::vector<int>(l.N - 1);
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

void Solver::uni_crossover(Bvec& target, const Bvec& first, const Bvec & second) {
	for (int i = 0; i < labs.N; i++) {
		int rnd = fair_coin(gen);
		if (rnd % 2 == 0) target.set(i, first.get(i));
		else target.set(i, second.get(i));
	}
}

int Solver::get_N() { return labs.N; }
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

void Solver::init_flip_value(const Bvec &S) {
	// construct Tau(S)
	for(int i = 0; i < labs.N - 1; ++i) {
		int row_size = labs.N - 1 - i;
		//tau[i] = std::vector<int>(row_size);
		for(int j = 1; j <= row_size; ++j) {
			tau[i][j - 1] = (S.get(j - 1)? 1: -1) * (S.get(i + j) ? 1 : -1);
		}
	}
	// construct C(S)
	for(int i = 0; i < labs.N - 1; i++) {
		c_s[i] = labs.corr(S.size(), i + 1, S);
	}
}

double Solver::flip_value(const Bvec &S, int i) {
	int f = 0;
	for(int p = 0; p < labs.N - 1; ++p) {
		int v = c_s[p];
		if(p <= labs.N - i) v = v - 2 * tau[p][i];
		if(p < i) v = v - 2  * tau[p][i - 1 -  p ];
		f = f + v * v;
	}
	labs.calls_num++;
	return (labs.N * labs.N)/ (double) (2 * f);
}
