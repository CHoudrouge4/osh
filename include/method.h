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
	SA(Labs, const double mu, const double t0);
	SA(const SA&);
	SA* clone() const;

	std::string get_name() const;

	bool run(long long timeout);

private:
	double mu; // linear cooling constant
	double t0 = 1000; // initial temprature
};


class TS : public Solver {

public:
	TS(Labs);
	TS(const TS&);
	TS* clone() const;

	std::string get_name() const;

	bool run(long long timeout);
	bool runOnce(); // Runs once


	void set_S(const Bvec s);
	void test_flip_val();

private:
	int max_itr; // TODO Use timeout
	std::vector<int> M; // Tenure table, holds tabu data
	Bvec S; // Temporary vector

	// If use_timeout is set, it'll use the timeout and work as a
	// normal solver. Otherwise it does max_itr.
	bool runInternal(long long timeout, bool use_timeout);
};

class SALS : public Solver {
public:
	SALS(Labs);
	SALS(const SALS& s);
	Solver* clone() const;
	std::string get_name() const;

	bool run(long long timeout);
	bool runOnce(); // Runs once

private:
	Bvec current;
	bool runInternal(long long timeout, bool use_timeout);
};

class MA : public Solver {

public:
	MA(Labs, bool isTS);
	MA(const MA&);
	MA* clone() const;

	std::string get_name() const;

	bool run(long long timeout);

private:
	// This is so conceptually wrong that I won't even comment on it
	bool isTS; // if true, we use ts as a subroutine, otherwise sals
	TS ts;
	SALS sals;

	const int popsize;
	const double px;
	const double pm;

	std::vector<Bvec> ppl;
	std::vector<Bvec> offsprings;
	std::vector<double> ppl_val;
	std::vector<double> off_val;

	Bvec& select_parent();
};
