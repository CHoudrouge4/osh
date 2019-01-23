#pragma once
#include "labs.h"
#include "solver.h"
#include <vector>
#include <random>
#include <iostream>

class Method : public Solver {

public:
	Method(const Labs&);

	bvec mu_lambda(int mu, int lambda, int iterN);
	bvec one_plus_one(int iterN);
	void simulating_annealing(double t, int nb_iterations);

private:
	std::vector<bool> get_neighbor();
	void cooling(double&, int);
	double compute_acceptance_probability(double, double, double);
};
