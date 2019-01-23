#pragma once
#include "labs.h"
#include <vector>
#include <random>
#include <iostream>

class Solver {
public:
	Solver(const Labs&);

	void sbm(bvec&, int);
	void sbm(bvec&);

protected:
	Labs labs;

	double opt;
	bvec S;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> dis;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
