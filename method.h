#pragma once
#include "labs.h"
#include "solver.h"
#include <vector>
#include <random>
#include <iostream>
#include "bvec.h"

class OnePlusOne : public Solver {
	// Temporary optimum, the other "1"
	Bvec tmp_opt;
	// Number of iterations to perform
	int iterN;

public:
	OnePlusOne(Labs);
	bool run(long long timeout);
};


class MuLambda : public Solver {
	// Algo parameters
	const int mu;
	const int lambda;
	const int crossover_prob;
	std::uniform_int_distribution<int> uni_dis_mu;

	// population, consists of mu + lambda elements
	std::vector<Bvec> ppl;

public:
	MuLambda(Labs, int mu, int lambda, double crossover_prob);
	bool run(long long timeout);
};


class SA : public Solver {

public:
	SA(Labs, const double alpha, const double mu);

	bool run(long long timeout);
	void set_cooling_option(bool is_exp);
	void set_initial_tempreature(double init_temp);
private:

	const double alpha;        // cooling exponentiatl constant
	const double mu;           // cooling linear constant
	double t0 = 1000;    // initial temprature
	// it must be an enum, but for now we only have 2 options
	bool exp_cooling = true; // otherwise linear
};


class TS : public Solver {

public:
	TS(Labs, const int itr);

	bool run(long long timeout);
	void set_max_itr(const int);
	void set_S(const Bvec s);
private:
	int max_itr;
	std::vector<int> M;
	Bvec S;
};

class MA : public Solver {

public:
	MA(Labs, const int, const double, const double);
	bool run(long long timeout);
	void get_optimums();
	void set_max_itr(const int itr);

private:

	double px;
	double pm;
	int max_itr = 1000;

	std::vector<Bvec> ppl;
	std::vector<Bvec> offsprings;
	std::vector<double> ppl_val;
	std::vector<double> off_val;

	Bvec select_parent();
	void replace();
};
