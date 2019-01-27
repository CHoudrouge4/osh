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

void OnePlusOne::run(int iterN) {
	opt_val.randomise();
	tmp_opt = opt_val;

	recordBegin();
	for (int i = 0; i < iterN; i++) {
		sbm(tmp_opt);
		if(labs.F(tmp_opt) > labs.F(opt_val)) {
			opt_val = tmp_opt;
			opt = labs.F(opt_val);
		}
		else tmp_opt = opt_val;
		recordCurrent(i);
	}
}

void OnePlusOne::reset() { opt_val.clear(); opt = labs.F(opt_val); }


// Mu Lambda


MuLambda::MuLambda (const Labs& labs, int mu, int lambda, double crossoverP):
	Solver(labs), mu(mu), lambda(lambda), crossoverP(crossoverP) {
	uni_dis_mu = std::uniform_int_distribution<int>(0, mu-1);
	ppl = std::vector<Bvec>(mu+lambda, Bvec(labs.N));
}

void MuLambda::run(int iterN) {
	for (int i = 0; i < mu; i++) { ppl[i].randomise(); }

	recordBegin();
	for (int i = 0; i < iterN; i++) {
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
		recordCurrent(i);
	}
}

void MuLambda::reset() {
	opt_val.clear();
}


// Simulated annealing


SA::SA(const Labs& labs, const double alpha, const double mu) :
	Solver(labs), opt(labs.N) {
	assert(alpha > 0 and alpha < 1);
	assert(mu > 0 and  mu < 1);
	this->alpha = alpha;
	this->mu = mu;
}

Bvec SA::get_neighbor() {
	Bvec neighbor(labs.N);
	sbm(neighbor, 1);
	return neighbor;
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

void SA::simulating_annealing(double t, int nb_iterations, char option) {
	std::uniform_real_distribution<> urd(0, 1);
	Bvec s(labs.N);
	s.randomise();
	opt = Bvec(labs.N);
	for(int i = 0; i < nb_iterations; ++i) {
		auto neighbor = get_neighbor();
		double fs = labs.F(s);
		double fn = labs.F(neighbor);
		if(fn > fs) {
			s = neighbor;
			if(labs.F(opt) < fn)
				opt = s;
		} else {
			double acc_prob = compute_acceptance_probability(fs, fn, t);
			double prob = urd(gen);
			if(prob < acc_prob) s = neighbor;
		}
		cooling(option, t, i);
	}
}
