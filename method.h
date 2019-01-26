#pragma once
#include "labs.h"
#include "solver.h"
#include <vector>
#include <random>
#include <iostream>


class OnePlusOne : public Solver {
private:
	// Temporary optimum, the other "1"
	Bvec tmp_opt;

public:
	OnePlusOne(const Labs&);
	void run(int iterNum);
	void reset();
};


class MuLambda : public Solver {
private:
	// Algo parameters
	const int mu;
	const int lambda;
	// should be also constant but I'm not a c++ wizard
	// and I get really sad at how gcc shows me errors;
	// I don't even want to read that much text, sorry
	// ghc <3
	std::uniform_int_distribution<int> uni_dis_mu;

	// our "main" population
	std::vector<Bvec> xs;
	// "temporary" population
	std::vector<Bvec> ys;

public:
	MuLambda(const Labs&, int mu, int lambda);
	void run(int iterNum);
	void reset();
};

class SA : public Solver {

	public:
	//	SA(const Labs& labs) : Solver(labs) {}
		using Solver::Solver;
		//Bvec one_plus_one(int iterN);
		void simulating_annealing(double t, int nb_iterations);

	private:
		Bvec current;
		Bvec temp;

		Bvec get_neighbor();
		void cooling(double&, int);
		double compute_acceptance_probability(double, double, double);
};
