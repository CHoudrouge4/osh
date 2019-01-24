#include "method.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>


MuLambda::MuLambda (const Labs& labs, int mu, int lambda):
	Solver(labs), mu(mu), lambda(lambda) {
	uni_dis_mu = std::uniform_int_distribution<int>(0, mu-1);
	xs = std::vector<Bvec>(mu, Bvec(labs.N));
	ys = std::vector<Bvec>(lambda, Bvec(labs.N));
}

void MuLambda::run(int iterN) {

	for (int i = 0; i < mu; i++) { xs[i].randomise(); }

	for (int i = 0; i < iterN; i++) {
		// variation
		for (int j = 0; j < lambda; j++) {
			ys[j] = xs[uni_dis_mu(gen)];
			sbm(ys[j]);
		}
		// selection (sort descending)
		std::sort(ys.begin(), ys.end(),
				  [this](Bvec a, Bvec b) { return labs.F(a) > labs.F(b); });

		for (int j = 0; j < mu; j++) xs[j] = ys[j];
	}

	// We could do it in the xs[j] = ys[j] loop btw
	auto maxValue =
		std::max_element(xs.begin(), xs.end(),
						 [this](Bvec a, Bvec b) { return labs.F(a) < labs.F(b); });
	opt_val = *maxValue;
}

void MuLambda::reset() {
	opt_val.clear();
}

//Method::Method (const Labs& labs): Solver(labs) {
//}

//Bvec Method::one_plus_one(int iterN) {
//	current = labs.random_Bvec();
//	for (int i = 0; i < iterN; i++) {
//		temp = current;
//		sbm(temp);
//		if(labs.F(temp) > labs.F(current)) current = temp;
//	}
//}


//std::vector<bool> Method::get_neighbor() {
//	std::vector<bool> neighbor = S;
//	sbm(neighbor, 1);
//	return neighbor;
//}
//
//double Method::compute_acceptance_probability(double fs, double fn, double t) {
//	return std::exp(-(fs - fn)/t);
//}
//
//// we have to try different functions
//void Method::cooling(double &t, int i) {
//	t = std::max(1.0, t - t/i);
//}
//
//void Method::simulating_annealing(double t, int nb_iterations) {
//	std::uniform_real_distribution<> urd(0, 1);
//	Bvec s = labs.random_Bvec();
//	Bvec opt(labs.N);
//	for(int i = 0; i < nb_iterations; ++i) {
//		auto neighbor = get_neighbor();
//		double fs = labs.F(s);
//		double fn = labs.F(neighbor);
//		if(fn > fs) {
//			s = neighbor;
//			if(labs.F(opt) > fn)
//				opt = s;
//		} else {
//			double acc_prob = compute_acceptance_probability(fs, fn, t);
//			double prob = urd(gen);
//			if(prob < acc_prob) s = neighbor;
//		}
//		cooling(t, i);
//	}
//}
