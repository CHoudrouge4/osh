#include "method.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <limits>

// 1 + 1


OnePlusOne::OnePlusOne(const Labs& labs): Solver(labs), tmp_opt(labs.N) {}

bool OnePlusOne::run(long long timeout) {
	opt_val.randomise();
	opt = labs.F(opt_val);
	tmp_opt = opt_val;

	recordBegin();
	for (int i = 0; true; i++) {
		sbm(tmp_opt);
		if(labs.F(tmp_opt) > labs.F(opt_val)) {
			opt_val = tmp_opt;
			opt = labs.F(opt_val);
		}
		else tmp_opt = opt_val;

		if (opt == labs.optF) { running_time = getRunningTimeMs(); return true; }
		if (getRunningTimeMs() > timeout) { running_time = getRunningTimeMs(); return false; }
		if (i % 100 == 0) recordCurrent();
	}
}

// Mu Lambda


MuLambda::MuLambda (const Labs& labs, int mu, int lambda, double crossover_prob)
	: Solver(labs)
	, mu(mu)
	, lambda(lambda)
	, crossover_prob(crossover_prob)
	, uni_dis_mu(std::uniform_int_distribution<int>(0, mu-1)) {
	ppl = std::vector<Bvec>(mu+lambda, Bvec(labs.N));
}

bool MuLambda::run(long long timeout) {
	for (int i = 0; i < mu; i++) { ppl[i].randomise(); }

	recordBegin();
	for (int i = 0; true; i++) {
		// variation
		for (int j = 0; j < lambda; j++) {
			if (uni_dis_one(gen) < crossover_prob) {
				ppl[mu+j] = ppl[uni_dis_mu(gen)];
				uni_crossover(ppl[mu+j], ppl[uni_dis_mu(gen)]);
			} else {
				ppl[mu+j] = ppl[uni_dis_mu(gen)];
				sbm(ppl[mu+j]);
			}
		}
		// selection (sort descending)
		std::sort(ppl.begin(), ppl.end(),
				  [this](Bvec a, Bvec b) { return labs.F(a) > labs.F(b); });

		opt_val = ppl[0];
		opt = labs.F(opt_val);
		if (opt == labs.optF) { running_time = getRunningTimeMs(); return true; }
		if (getRunningTimeMs() > timeout) { running_time = getRunningTimeMs(); return false; }
		if (i % 10 == 0) recordCurrent();
	}
}

// Simulated annealing

SA::SA(const Labs& labs, const double alpha, const double mu) :
	Solver(labs) {
	assert(alpha > 0 and alpha < 1);
	assert(mu > 0 and  mu < 1);
	this->alpha = alpha;
	this->mu = mu;
}

bool SA::run(long long timeout) {
	double t = t0;
	Bvec s(labs.N);
	sbm(s, labs.N);
	opt = labs.F(opt_val);

	recordBegin();
	Bvec neighbor = s;
	for (int i = 0; true; i++) {
		neighbor = s;
		sbm(neighbor, 1);
		double fs = labs.F(s);
		double fn = labs.F(neighbor);
		if(fn > fs) {
			s = neighbor;
			if(opt < fn) {
				opt_val = s;
				opt = fn;
				recordCurrent();
			}
		} else {
			// acceptance probability
			double acc_prob = std::exp(-(fs - fn)/t);
			double prob = uni_dis_one(gen);
			if(prob < acc_prob) s = neighbor;
		}

		if (i % 150 == 0) recordCurrent(); // we don't want to use it too often
		if (opt == labs.optF) { running_time = getRunningTimeMs(); return true; }
		if (getRunningTimeMs() > timeout) { running_time = getRunningTimeMs(); return false; }

		// Cooling
		if (exp_cooling) {
			t = t * std::pow(alpha, i);
		} else {
			t = t - mu * i;
		}
	}
}

void SA::set_cooling_option(bool is_exp) { exp_cooling = is_exp; }
void SA::set_initial_tempreature(double init_temp) { t0 = init_temp; }

TS::TS(const Labs& l, const int itr) :
	 Solver(l) {
	this->max_itr = itr;
	assert(max_itr >= 1);
	assert(max_itr == itr);
	M = std::vector<int>(l.N, 0);
	opt_val = Bvec(labs.N);
	S = Bvec(labs.N);
	sbm(S);
	L = l.N;
}

bool TS::run(long long timeout) {

	const int min_tabu = max_itr/10;
	const int extra_tabu = max_itr/50;
	std::uniform_int_distribution<int> urand(0, std::max(extra_tabu - 1, 1));
	opt_val = S;
	Bvec S_plus = Bvec(labs.N);
	opt = labs.F(opt_val);
	int index = 0;
	for(int k = 1; k < max_itr; ++k) {
		double f_plus = std::numeric_limits<double>::min();
		for(int i = 0; i < L; ++i ) {
			Bvec neighbor = S;
			sbm(neighbor, 1);
			double f_neighbour = labs.F(neighbor);
			if(k >= M[i] || f_neighbour > opt) {
				if(f_neighbour > f_plus) {
					f_plus = f_neighbour;
					S_plus = neighbor;
					index = i;
				}
			}
		}

		S = S_plus;
		M[index] = k + min_tabu + urand(gen);
		if(f_plus > opt) {
			opt_val = S_plus;
			opt = f_plus;
		}
	}
	return false;
}

void TS::set_max_itr(const int itr) { max_itr = itr; }
void TS::set_S(const Bvec s) { S = s; }

