#pragma once
#include "labs.h"
#include "solver.h"
#include <vector>
#include <random>
#include <iostream>


class OnePlusOne : public Solver {
	// Temporary optimum, the other "1"
	Bvec tmp_opt;
	// Number of iterations to perform
	int iterN;

public:
	OnePlusOne(const Labs&);
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
	MuLambda(const Labs&, int mu, int lambda, double crossover_prob);
	bool run(long long timeout);
};

class SA : public Solver {

public:
	SA(const Labs&, const double alpha, const double mu);

	bool run(long long timeout);
	void set_cooling_option(bool is_exp);
	void set_initial_tempreature(double init_temp);
private:

	double alpha;        // cooling exponentiatl constant
	double mu;           // cooling linear constant
	double t0 = 1000;    // initial temprature
	// it must be an enum, but for now we only have 2 options
	bool exp_cooling = true; // otherwise linear
};

class TS : public Solver {
public:
	TS(const Labs&, const int max_itr);

	bool run(long long timeout);
	void set_max_itr(const int&);

private:
	int max_itr;
	int L;
	std::vector<int> M;
};
