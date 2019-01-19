#pragma once
#include "labs.h"
#include <vector>
#include <random>
#include <iostream>

class Methods {

public:
	Methods(const Labs&);

	bvec mu_lambda(int mu, int lambda, int iterN);
	bvec one_plus_one(int iterN);

	void sbm(bvec&, int);
	void sbm(bvec&);

	void simulating_annealing(double t, int nb_iterations);
	std::vector<bool> get_neighbor();
	void cooling(double&, int);
	double compute_acceptance_probability(double, double, double);

private:
	Labs labs;

	double opt;
	bvec S;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> dis;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
