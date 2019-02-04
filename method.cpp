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


OnePlusOne::OnePlusOne(Labs labs): Solver(labs), tmp_opt(labs.N) {}

bool OnePlusOne::run(long long timeout) {
	sbm(opt_val);
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


MuLambda::MuLambda (Labs labs, int mu, int lambda, double crossover_prob)
	: Solver(labs)
	, mu(mu)
	, lambda(lambda)
	, crossover_prob(crossover_prob)
	, uni_dis_mu(std::uniform_int_distribution<int>(0, mu-1)) {
	ppl = std::vector<Bvec>(mu+lambda, Bvec(labs.N));
}

bool MuLambda::run(long long timeout) {
	for (int i = 0; i < mu; i++) { sbm(ppl[i]); }

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

SA::SA(Labs labs, const double alpha, const double mu) :
	Solver(labs), alpha(alpha), mu(mu) {
	assert(alpha > 0 and alpha < 1);
	assert(mu > 0 and  mu < 1);
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

TS::TS(Labs l, const int itr) :
	Solver(l), S(labs.N) {
	this->max_itr = itr;
	assert(max_itr >= 1);
	assert(max_itr == itr);
	M = std::vector<int>(l.N, 0);
	opt_val = Bvec(labs.N);
	sbm(S);
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
		for(int i = 0; i < labs.N; ++i ) {
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

MA::MA(Labs l, const int popsize, const int off_size, const double ppx, const double ppm) : Solver(l) {
	ppl = std::vector<Bvec> (popsize, Bvec(l.N));
	ppl_val = std::vector<double>(popsize);
	offsprings = std::vector<Bvec> (off_size, Bvec(l.N));
	off_val = std::vector<double>(popsize);
	px = ppx;
	pm = ppm;
}

bool MA::run(long long timeout) {
	for(size_t i = 0; i < ppl.size(); ++i) {
		sbm(ppl[i]);
		ppl_val[i] = labs.F(ppl[i]);
	}

	TS local_search(this->labs, 1000);

	recordBegin();
	while(getRunningTimeMs() < timeout) {

		for(size_t i = 0; i < offsprings.size(); ++i) {
			double rnd = uni_dis_one(gen);
			if(rnd < px) {
				auto parent1 = select_parent();
				auto parent2 = select_parent();
				uni_crossover(offsprings[i], parent1);
				uni_crossover(offsprings[i], parent2);
			} else {
				offsprings[i] = select_parent();
			}
			rnd = uni_dis_one(gen);
			if(rnd < pm) {
				sbm(offsprings[i]);
			}
			local_search.set_S(offsprings[i]);
			local_search.run(timeout);
			offsprings[i] = local_search.getOptimal();
			off_val[i] = local_search.get_opt();
		}

		running_time = getRunningTimeMs();
	}

	return false;
}

Bvec MA::select_parent() {
	int rnd_index = uni_dis_N(gen);
	return ppl[rnd_index];
}


void MA::replace() {
	for(size_t i = 0; i < ppl.size(); ++i)
		if(off_val[i] >= ppl_val[i])
			ppl[i] = offsprings[i];
}

void MA::get_optimums() {
	opt = ppl_val[0];
	opt_val = ppl[0];
	for(size_t i = 1; i < ppl.size(); ++i) {
		if(opt < ppl_val[i]) {
			opt = ppl_val[i];
			opt_val = ppl[i];
		}
	}
}
