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

SA::SA(Labs labs, const double alpha, const double mu) :
	Solver(labs), alpha(alpha), mu(mu) {
	assert(alpha > 0 && alpha < 1);
	assert(mu > 0 &&  mu < 1);
}

SA::SA(const SA& s) : SA(s.labs, s.alpha, s.mu) { t0 = s.t0; }

SA* SA::clone() const { return new SA(*this); }

std::string SA::get_name() const { return "sa"; }

bool SA::run(long long timeout) {
	double t = t0;
	Bvec s(labs.N);
	sbm(s, labs.N);
	opt = labs.F(opt_vec);

	record_begin();
	Bvec neighbor = s;
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

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (i % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}

		// Cooling
		if (linear_cooling) {
			t = t - mu * i;
		} else {
			t = t * std::pow(alpha, i);
		}
	}
}

void SA::set_cooling_option(bool is_lin) { linear_cooling = is_lin; }
void SA::set_initial_tempreature(double init_temp) { t0 = init_temp; }

void SA::select_params() {
	std::vector<interval> constraints = get_constraints();
	std::vector<double> val(get_param_size());
	init_value(val, constraints);
	const double epsilon = 0.3;
	const double beta = 0.3;
	const int64_t timeout = 1000;
	double pre_opt = opt;
	int i = 0;
	while(true) {
		one_step(val, constraints, beta, epsilon, timeout);
		if(std::abs(opt - pre_opt) <= 0.1 || i == 1000) { break;}
		pre_opt = opt;
		i++;
	}
	set_params(val);
}

void SA::one_step(std::vector<double> &val
				  , const std::vector<interval> &constraints
				  , double beta
				  , double epsilon
				  , const int64_t timeout) {
	for(size_t j = 0; j < val.size(); ++j) {
		double df = finite_difference(val, j, epsilon, timeout);
		val[j] += beta * df;
		val[j] = std::min(val[j], constraints[j].second - epsilon);
		val[j] = std::max(val[j], constraints[j].first + epsilon);
	}
	set_params(val);
	run(timeout);
}

double SA::finite_difference(std::vector<double> &val
							 , const size_t j
							 , const double epsilon
							 , const int64_t timeout) {
	set_params(val);
	run(timeout);
	double f = opt;
	val[j] += epsilon;
	set_params(val);
	run(timeout);
	val[j] -= epsilon;
	double fh = opt;
	return (fh - f) / epsilon;
}

void SA::set_params(std::vector<double> & params) {
	if(linear_cooling) {
		mu = params[0];
	} else {
		alpha = params[0];
	}
	t0 = params[1];
}

int SA::get_param_size() {
	return int(2);
}

void SA::init_value(std::vector<double> &val, std::vector<interval> &intervals) {
	for(size_t i = 0; i < val.size(); ++i) {
		std::uniform_real_distribution<> dis(intervals[i].first, intervals[i].second);
		val[i] = dis(gen);
	}
}

std::vector<interval> SA::get_constraints() {
		std::vector<interval> intervals(get_param_size());
		intervals[0] = {0, 1};
		intervals[1] = {0, 10000000};
		return intervals;
}

std::vector<double> SA::get_params() {
	std::vector<double> v(get_param_size());
	if(linear_cooling) v[0] = mu;
	else v[0] = alpha;
	v[1] = t0;
	return v;
}



// Tabu search

TS::TS(Labs l, const int max_itr) : Solver(l), max_itr(max_itr), S(labs.N) {
	assert(max_itr >= 1);
	M = std::vector<int>(l.N, 0);
	opt_vec = Bvec(labs.N);
	sbm(S);
}

TS::TS(const TS& s) : TS(s.labs, s.max_itr) {
	this->M = s.M;
	this->opt_vec  = s.opt_vec;
	this->S = s.S;
}
TS* TS::clone() const { return new TS(*this); }
std::string TS::get_name() const { return "tabu"; }

bool TS::runInternal(long long timeout, bool use_timeout) {
	const int min_tabu = max_itr/10;
	const int extra_tabu = max_itr/50;
	std::uniform_int_distribution<int> urand(0, std::max(extra_tabu - 1, 1));
	record_begin();
	for(int i = 0; use_timeout ? true : (i < max_itr); ++i) {
		Bvec opt_vec_current = S;
		double opt_current = labs.F(opt_vec_current);
		Bvec S_plus = Bvec(labs.N);
		int index = 0;
		for(int k = 1; k <= max_itr; ++k) {
			double f_plus = std::numeric_limits<double>::min();
			for(int i = 0; i < labs.N; ++i) {
				Bvec neighbor = S;
				neighbor.flip_bit(i);
				double f_neighbour = labs.F(neighbor);
				if(k >= M[i] || f_neighbour > opt_current) {
					if(f_neighbour > f_plus) {
						f_plus = f_neighbour;
						S_plus = neighbor;
						index = i;
					}
				}
			}

			S = S_plus;
			M[index] = k + min_tabu + urand(gen);
			if(f_plus > opt_current) {
				opt_vec_current = S_plus;
				opt_current = f_plus;
			}
		}

		if(opt_current > opt) {
			opt = opt_current;
			opt_vec = opt_vec_current;
			record_current();
		}

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (i % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}
	}
	return false;
}

bool TS::run(long long timeout) { return runInternal(timeout,true); }

void TS::set_max_itr(const int itr) { max_itr = itr; }
void TS::set_S(const Bvec s) { S = s; }

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




// Memetic algorithm

MA::MA(Labs l)
	: Solver(l)
	, popsize(100)
	, px(0.9)
	, pm(1/((double)l.N)) {
	ppl = std::vector<Bvec> (popsize, Bvec(l.N));
	ppl_val = std::vector<double>(popsize);
	offsprings = std::vector<Bvec> (popsize, Bvec(l.N));
	off_val = std::vector<double>(popsize);
}

MA::MA(const MA& s) : MA(s.labs) { }

MA* MA::clone() const { return new MA(*this); }

std::string MA::get_name() const { return "ma"; }

bool MA::run(long long timeout) {
	for(size_t i = 0; i < ppl.size(); ++i) {
		sbm(ppl[i]);
		ppl_val[i] = labs.F(ppl[i]);
	}

	int max_itr = 1000; // TODO set up to random

	TS local_search(this->labs, max_itr);

	// TODO record_current is broken! We don't record cntProb!

	record_begin();
	for(int k = 0; true; ++k) {
		for(size_t i = 0; i < offsprings.size(); ++i) {
			double rnd = uni_dis_one(gen);
			if(rnd <= px) {
				auto parent1 = select_parent();
				auto parent2 = select_parent();
				uni_crossover(offsprings[i], parent1, parent2);
			} else offsprings[i] = select_parent();

			rnd = uni_dis_one(gen);
			if(rnd <= pm) sbm(offsprings[i]);

			local_search.set_S(offsprings[i]);
			local_search.runInternal(0, false);
			offsprings[i] = local_search.get_opt_vec();
			off_val[i] = local_search.get_opt();
		}

		// Replace
		for(size_t i = 0; i < ppl.size(); ++i) {
			if (off_val[i] >= ppl_val[i]) {
				ppl[i] = offsprings[i];
				ppl_val[i] = off_val[i];
			}
		}

		// Get optimums
		auto newOpt = ppl_val[0];
		if (newOpt > opt) { record_current(); }
		opt = newOpt;
		int index = 0;
		for(size_t i = 1; i < ppl.size(); ++i) {
			if(opt < ppl_val[i]) {
				opt = ppl_val[i];
				index = i;
			}
		}
		opt_vec = ppl[index];


		if (opt == labs.optF) { running_time = get_running_time_ms(); return true; }
		if (k % 50 == 0 && get_running_time_ms() > timeout) {
			running_time = get_running_time_ms();
			return false;
		}
	}
}

Bvec& MA::select_parent() {
	int rnd_index = uni_dis_N(gen);
	return ppl[rnd_index];
}

// Steepest ascent local search


SALS::SALS(const Labs & l) : Solver(l), current(l.N) {
	sbm(current);
}

SALS::SALS(const SALS& s) : SALS(s.labs) {
	this->opt = s.opt;
	this->opt_vec = s.opt;
	this->current = s.current;
}

bool SALS::run(long long timeout) {

	double tmp_opt = labs.F(current);
	init_flip_value(current);
	Bvec S_plus(labs.N);
	record_begin();
	for(int i = 0; true; ++i) {
		double f_plus = std::numeric_limits<double>::min();
		for(int j = 0; j < labs.N; ++j) {
			current.flip_bit(j);
			Bvec neighbor = current;
			double f_neighbour = flip_value(j);
			current.flip_bit(j);
			if(f_neighbour > f_plus) {
				f_plus = f_neighbour;
				S_plus = neighbor;
			}

			if(f_plus > tmp_opt) {
				tmp_opt = f_plus;
				current = S_plus;
				init_flip_value(current);
				record_current();
			}
		}

		if(tmp_opt >= opt) {
			opt_vec = current;
			opt = tmp_opt;
		}

		if (opt == labs.optF) { running_time = get_running_time_ms(); return true;}
		if (i % 50 == 0 && (get_running_time_ms() > timeout)) {
			running_time = get_running_time_ms();
			return false;
		}
		sbm(current);
		init_flip_value(current);
	}
}

std::string SALS::get_name() const { return "sals"; }
Solver* SALS::clone() const { return new SALS(*this); }
