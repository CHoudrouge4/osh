#include "method.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>


Method::Method (const Labs& labs): Solver(labs) {
}

bvec Method::one_plus_one(int iterN) {
	bvec x = labs.random_bvec();
	for (int i = 0; i < iterN; i++) {
		bvec y = x;
		sbm(y);
		if(labs.F(y) > labs.F(x)) x = y;
	}
	return x;
}

bvec Method::mu_lambda(int mu, int lambda, int iterN) {
	std::uniform_int_distribution<> dis(0, mu-1);

	// population
	std::vector<bvec> xs(mu);
	for (int i = 0; i < mu; i++) xs[i] = labs.random_bvec();

	std::vector<bvec> ys(mu);

	for (int i = 0; i < iterN; i++) {
		// variation
		for (int j = 0; j < lambda; j++) {
			ys[j] = xs[dis(gen)];
			sbm(ys[j]);
		}
		// selection (sort descending)
		std::sort(ys.begin(), ys.end(), [this](bvec a, bvec b) { return labs.F(a) < labs.F(b); });
		for (int j = 0; j < mu; j++) xs[j] = ys[j];
	}

	auto maxValue =
		std::max_element(xs.begin(), xs.end(),
						 [this](bvec a, bvec b) { return labs.F(a) > labs.F(b); });
	return *maxValue;
}
std::vector<bool> Method::get_neighbor() {
	std::vector<bool> neighbor = S;
	sbm(neighbor, 1);
	return neighbor;
}

double Method::compute_acceptance_probability(double fs, double fn, double t) {
	return std::exp(-(fs - fn)/t);
}

// we have to try different functions
void Method::cooling(double &t, int i) {
	t = std::max(1.0, t - t/i);
}

void Method::simulating_annealing(double t, int nb_iterations) {
	std::uniform_real_distribution<> urd(0, 1);
	bvec s = labs.random_bvec();
	bvec opt(labs.N);
	for(int i = 0; i < nb_iterations; ++i) {
		auto neighbor = get_neighbor();
		double fs = labs.F(s);
		double fn = labs.F(neighbor);
		if(fn > fs) {
			s = neighbor;
			if(labs.F(opt) > fn)
				opt = s;
		} else {
			double acc_prob = compute_acceptance_probability(fs, fn, t);
			double prob = urd(gen);
			if(prob < acc_prob) s = neighbor;
		}
		cooling(t, i);
	}
}
