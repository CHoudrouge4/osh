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
	while (true) {
		sbm(tmp_opt);
		if(labs.F(tmp_opt) > labs.F(opt_val)) {
			opt_val = tmp_opt;
			opt = labs.F(opt_val);
		}
		else tmp_opt = opt_val;

		recordCurrent();
		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;
	}
}

// Mu Lambda


MuLambda::MuLambda (const Labs& labs, int mu, int lambda, double crossoverP)
	: Solver(labs)
	, mu(mu)
	, lambda(lambda)
	, crossoverP(crossoverP)
	, uni_dis_mu(std::uniform_int_distribution<int>(0, mu-1)) {
	ppl = std::vector<Bvec>(mu+lambda, Bvec(labs.N));
}

void MuLambda::run(long long timeout) {
	for (int i = 0; i < mu; i++) { ppl[i].randomise(); }

	recordBegin();
	while (true) {
		// variation
		for (int j = 0; j < lambda; j++) {
			if (uni_dis_one(gen) < crossoverP) {
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
		recordCurrent();
		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;
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

Bvec SA::get_neighbor(Bvec s) {
	sbm(s, 1);
	return s;
}

double SA::compute_acceptance_probability(double fs, double fn, double t) {
	return std::exp(-(fs - fn)/t);
}

// we have to try different functions
void SA::cooling(char option, double &t, int i) {
	switch(option) {
		case 'e': exp_cooling(t, i);
		case 'l': linear_cooling(t, i);
	}
}

void SA::exp_cooling(double &t, int i) {
	t = t * std::pow(alpha, i);
}

void SA::linear_cooling(double &t, int i) {
	t = t - mu * i;
}

void SA::simulating_annealing(double t, long long timeout, char option) {

	Bvec s(labs.N);
	sbm(s, labs.N);
	std::cout << "what is s: " << s << '\n';
	opt = labs.F(opt_val);

	recordBegin();
	for (int i = 0; true; i++) {
		auto neighbor = get_neighbor(s);
		double fs = labs.F(s);
		double fn = labs.F(neighbor);
		if(fn > fs) {
			s = neighbor;
			if(opt < fn) {
				opt_val = s;
				opt = fn;
			}
		} else {
			double acc_prob = compute_acceptance_probability(fs, fn, t);
			double prob = uni_dis_one(gen);
			if(prob < acc_prob) s = neighbor;
		}
		cooling(option, t, i);

		recordCurrent();
		if (opt == labs.optF) break;
		if (getRunningTimeMs() > timeout) break;
	}
}

void SA::run(long long timeout) {
	simulating_annealing(t0, timeout, option);
}

void SA::set_cooling_option(char cooling_option) { option = cooling_option; }
void SA::set_initial_tempreature(double init_temp) { t0 = init_temp; }
