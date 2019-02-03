#include "method.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cassert>


// 1 + 1


OnePlusOne::OnePlusOne(const Labs& labs): Solver(labs), tmp_opt(labs.N) {}

void OnePlusOne::run(long long timeout) {
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

		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;
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

void MuLambda::run(long long timeout) {
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
		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;
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

void SA::run(long long timeout) {
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
		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;

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
