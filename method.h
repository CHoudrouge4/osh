#pragma once
#include "labs.h"
#include "solver.h"
#include <vector>
#include <random>
#include <iostream>


class OnePlusOne : public Solver {
	// Temporary optimum, the other "1"
	Bvec tmp_opt;

public:
	OnePlusOne(const Labs&);
	void run(int iterNum);
	void reset();
};

class MuLambda : public Solver {
	// Algo parameters
	const int mu;
	const int lambda;
	const int crossoverP;
	// should be also constant but I'm not a c++ wizard
	// and I get really sad at how gcc shows me errors;
	// I don't even want to read that much text, sorry
	// ghc <3
	//
	//  "Happiness is Programming in C++"
	//							Hussein Houdrouge - 2016
	//
	std::uniform_int_distribution<int> uni_dis_mu;

	// population, consists of mu + lambda elements
	std::vector<Bvec> ppl;

public:
	MuLambda(const Labs&, int mu, int lambda, double crossoverP);
	void run(int iterNum);
	void reset();
};

class SA : public Solver {

	public:
		//using Solver::Solver;
		SA(const Labs&, const double alpha, const double mu);
		// options: l for linear and e for exponention.
		void simulating_annealing(double t, int nb_iterations, char option);

	private:
		Bvec opt;
//		Bvec temp;

		double alpha; // coolint exponentiatl constant
		double mu;    // coolint linear constant
		double t0;    // initial temprature

		Bvec get_neighbor();
		void cooling(char, double&, int);
		double compute_acceptance_probability(double, double, double);
		void exp_cooling(double&, int);
		void linear_cooling(double&, int);
};
