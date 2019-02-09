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
	OnePlusOne(const OnePlusOne&);
	OnePlusOne* clone() const;

	std::string get_name() const;

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
	MuLambda(const MuLambda&);
	MuLambda* clone() const;

	std::string get_name() const;

	bool run(long long timeout);
};


class SA : public Solver {

public:
	SA(Labs, const double alpha, const double mu);
	SA(const SA&);
	SA* clone() const;

	std::string get_name() const;

	bool run(long long timeout);

	void set_cooling_option(bool is_lin);
	void set_initial_tempreature(double init_temp);
private:

	double alpha; // cooling exponential constant
	double mu; // cooling linear constant
	double t0 = 1000; // initial temprature
	// it must be an enum, but for now we only have 2 options
	bool linear_cooling = true; // otherwise exponential
};


class TS : public Solver {

public:
	TS(Labs, const int itr);
	TS(const TS&);
	TS* clone() const;

	std::string get_name() const;

	bool run(long long timeout);

	void set_max_itr(const int);
	void set_S(const Bvec s);
	void test_flip_val();
private:
	int max_itr; // TODO Use timeout
	std::vector<int> M; // Tenure table, holds tabu data
	Bvec S; // Temporary vector
};

class MA : public Solver {

public:
	MA(Labs, const int, const double, const double);
	MA(const MA&);
	MA* clone() const;

	std::string get_name() const;

	bool run(long long timeout);
	void get_optimums();
	void set_max_itr(const int itr);

private:

	const int popsize;
	const double px;
	const double pm;
	int max_itr = 1000; // TODO use timeout we need for TS

	std::vector<Bvec> ppl;
	std::vector<Bvec> offsprings;
	std::vector<double> ppl_val;
	std::vector<double> off_val;

	Bvec select_parent();
	void replace();
};

class SALS : public Solver {
public:
	SALS(const Labs &);
	SALS(const SALS& s);

	bool run(long long timeout);
	std::string get_name() const;
	Solver* clone() const;
private:
	Bvec current;
};
