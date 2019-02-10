#include "method.h"
#include "labs.h"
#include <cstdlib>
#include <array>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cassert>
#include <limits>



// 1 + 1

OnePlusOne::OnePlusOne(Labs labs): Solver(labs), tmp_opt(labs.N) {}
OnePlusOne::OnePlusOne(const OnePlusOne& s): Solver(s), tmp_opt(s.labs.N) { }
OnePlusOne* OnePlusOne::clone() const { return new OnePlusOne(*this); }
std::string OnePlusOne::get_name() const { return "one_plus_one"; }

bool OnePlusOne::run(long long timeout) {
	sbm(opt_vec);
	opt = labs.F(opt_vec);
	tmp_opt = opt_vec;

	record_begin();
	for (int i = 0; true; i++) {
		sbm(tmp_opt);
		if(labs.F(tmp_opt) > labs.F(opt_vec)) {
			opt_vec = tmp_opt;
			opt = labs.F(opt_vec);
			record_current();
		}
		else tmp_opt = opt_vec;

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (i % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}
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

MuLambda::MuLambda(const MuLambda& s)
	: MuLambda(s.labs, s.mu, s.lambda, s.crossover_prob) { }

MuLambda* MuLambda::clone() const { return new MuLambda(*this); }

std::string MuLambda::get_name() const { return "mu_lambda"; }

bool MuLambda::run(long long timeout) {
	for (int i = 0; i < mu; i++) { sbm(ppl[i]); }

	record_begin();
	for (int i = 0; true; i++) {
		// variation
		for (int j = 0; j < lambda; j++) {
			if (uni_dis_one(gen) < crossover_prob) {
				ppl[mu+j] = ppl[uni_dis_mu(gen)];
				uni_crossover(ppl[mu+j], ppl[mu+j], ppl[uni_dis_mu(gen)]);
			} else {
				ppl[mu+j] = ppl[uni_dis_mu(gen)];
				sbm(ppl[mu+j]);
			}
		}
		// selection (sort descending)
		std::sort(ppl.begin(), ppl.end(),
				  [this](Bvec a, Bvec b) { return labs.F(a) > labs.F(b); });

		opt_vec = ppl[0];
		int opt1 = labs.F(opt_vec);
		if (opt1 > opt) {
			opt = opt1;
			record_current();
		}

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (i % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}
	}
}



// Simulated annealing

SA::SA(Labs labs, const double mu, const double t0) :
	Solver(labs), mu(mu), t0(t0) {
	assert(mu > 0 &&  mu < 1);
}

SA::SA(const SA& s) : SA(s.labs, s.mu, s.t0) { }

SA* SA::clone() const { return new SA(*this); }

std::string SA::get_name() const {
	//return "sa_" + std::to_string(mu) + "_" + std::to_string(t0);
	return "sa";
}

bool SA::run(long long timeout) {
	double t = t0;
	Bvec s(labs.N);
	sbm(s, labs.N);
	opt = labs.F(opt_vec);

	Bvec neighbor = s;
	record_begin();
	for (int i = 0; true; i++) {
		neighbor = s;
		sbm(neighbor, 1);
		double fs = labs.F(s);
		double fn = labs.F(neighbor);
		if(fn > fs) {
			s = neighbor;
			if(opt < fn) {
				opt_vec = s;
				opt = fn;
				record_current();
			}
		} else {
			// acceptance probability
			double acc_prob = std::exp(-(fs - fn)/t);
			double prob = uni_dis_one(gen);
			if(prob < acc_prob) s = neighbor;
		}

		// Linear cooling
		t = t - mu * i;

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (i % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}

	}
}



// Tabu search

TS::TS(Labs l) : Solver(l) {
	M = std::vector<int>(l.N, 0);
}
TS::TS(const TS& s) : TS(s.labs) { }
TS* TS::clone() const { return new TS(*this); }
std::string TS::get_name() const { return "tabu"; }

bool TS::runInternal(long long timeout, bool use_timeout, bool rand_S) {
	int var_factor = uni_dis_N(gen) / 2;
	int var_factor_sign = var_factor - var_factor / 2;
	// N +- N/4, paper suggests N +- N/2
	int max_itr = labs.N + var_factor_sign;

	const int min_tabu = max_itr/10;
	const int extra_tabu = max_itr/50;
	std::uniform_int_distribution<int> urand(0, std::max(extra_tabu - 1, 1));

	for (size_t i = 0; i < M.size(); i++) { M[i] = 0; }

	// otherwise use the start value provided
	if (rand_S) { opt_vec.randomise(); }
	opt = labs.F(opt_vec);

	// We use opt_vec instead of S*;
	Bvec S = opt_vec;

	record_begin();
	for(int k = 0; use_timeout ? true : (k < 10*max_itr); k++) {
		Bvec S_plus = S;
		int index = 0;

		init_flip_value(S);
		double f_plus = std::numeric_limits<double>::min();
		for(int i = 0; i < labs.N; ++i) {
			double f_neighbour = flip_value(i);
			if(k > M[i] || f_neighbour > opt) {
				if(f_neighbour > f_plus) {
					f_plus = f_neighbour;
					index = i;
				}
			}
		}
		S_plus.flip_bit(index);

		S = S_plus;

		M[index] = k + min_tabu + urand(gen);
		if(f_plus > opt) {
			opt_vec = S_plus;
			opt = f_plus;
			record_current();
		}

		if (opt == labs.optF) {
			running_time = get_running_time_ms(); return true; }
		if (k % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}

		// In timeout setting we sometimes re-randomise S to avoid being stuck.
		if (use_timeout && k > 0 && k % max_itr == 0) {
			S.randomise();
		}
	}
	return false;
}

bool TS::run(long long timeout) { return runInternal(timeout,true,true); }
void TS::runFromS(Bvec& s) { opt_vec = s; runInternal(0,false,false); }

void TS::test_flip_val() {
	for (int j = 0; j < 500; j++) {
		Bvec s(labs.N);
		s.randomise();

		init_flip_value(s);
		for (int i = 0; i < labs.N; i++) {
			s.flip_bit(i);
			double v = flip_value(i);
			//std::cout << v << " ~ " << labs.F(s) << "\n";
			assert(v == labs.F(s));
			s.flip_bit(i);
		}
	}
}


// Steepest ascent local search


SALS::SALS(Labs l) : Solver(l) { }

SALS::SALS(const SALS& s) : SALS(s.labs) { }

bool SALS::run(long long timeout) {

	opt_vec.randomise();
	opt = labs.F(opt_vec);

	Bvec S_plus(labs.N);

	Bvec tmp_vec = opt_vec;

	record_begin();
	for(int i = 0; true; i++) {
		double f_plus = std::numeric_limits<double>::min();

		sbm(tmp_vec);

		init_flip_value(tmp_vec);
		int index = 0;
		for(int j = 0; j < labs.N; ++j) {
			double f_neighbour = flip_value(j);
			if(f_neighbour > f_plus) {
				f_plus = f_neighbour;
				index = j;
			}
		}

		if(f_plus > opt) {
			opt_vec = tmp_vec;
			opt_vec.flip_bit(index);

			opt = f_plus;
			record_current();
		}


		if (opt == labs.optF) { running_time = get_running_time_ms(); return true;}
		if (i % 50 == 0 && (get_running_time_ms() > timeout)) {
			running_time = get_running_time_ms();
			return false;
		}
	}
}

std::string SALS::get_name() const { return "sals"; }
Solver* SALS::clone() const { return new SALS(*this); }



// Memetic algorithm

MA::MA(Labs l)
	: Solver(l)
	, ts(TS(l))
	, popsize(10)
	, offsize(50)
	, px(0.9)
	, pm(1/((double)l.N)) {
	ppl = std::vector<Bvec> (popsize, Bvec(l.N));
	ppl_val = std::vector<double>(popsize);
	offsprings = std::vector<Bvec> (offsize, Bvec(l.N));
	off_val = std::vector<double>(offsize);
	uni_dis_popsize = std::uniform_int_distribution<int>(0,popsize-1); // [0,popsize-1]
}

MA::MA(const MA& s) : MA(s.labs) { }

MA* MA::clone() const { return new MA(*this); }

std::string MA::get_name() const { return "ma"; }

bool MA::run(long long timeout) {

	// Cache for moving optimums from offsprings to population
	std::unordered_set<int> cache;

	for(size_t i = 0; i < ppl.size(); ++i) {
		ppl[i].randomise();
		ppl_val[i] = labs.F(ppl[i]);
	}

	opt_vec.randomise();
	opt = labs.F(opt_vec);

	record_begin();
	for(int k = 0; true; k++) {
		for(int i = 0; i < offsize; ++i) {
			double rnd = uni_dis_one(gen);
			if(rnd <= px) {
				single_point_crossover( offsprings[i]
										, ppl[uni_dis_popsize(gen)]
										, ppl[uni_dis_popsize(gen)]);
			} else {
				offsprings[i] = ppl[uni_dis_popsize(gen)];
			}

			if(uni_dis_one(gen) <= pm) sbm(offsprings[i], 2);

			ts.runFromS(offsprings[i]);
			labs.calls_num += ts.get_Labs().calls_num;
			assert(ts.get_opt() >= labs.F(offsprings[i]));
			offsprings[i] = ts.get_opt_vec();
			off_val[i] = ts.get_opt();
			ts.reset();
		}

		// Get optimums
		auto oldOpt = opt;

		// Replace (put 'popsize' optimal values from offsprings to pop)
		cache.clear();
		for(int i = 0; i < popsize; i++) {
			int best_index = 0;
			int best_val = 0;
			for (int j = 0; j < offsize; j++) {
				bool not_in_cache = cache.find(j) == cache.end();
				if (off_val[i] > best_val && not_in_cache) {
					best_index = i;
					best_val = off_val[i];
				}
			}
			cache.insert(best_index);
			ppl[i] = offsprings[best_index];
			ppl_val[i] = off_val[best_index];
		}

		opt_vec = ppl[0];
		opt = ppl_val[0];
		if (opt > oldOpt) { record_current(); }


		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (k % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}
	}
}
