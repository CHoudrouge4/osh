#pragma once
#include <vector>
#include <random>
#include <iostream>

using bvec = std::vector<bool>;

class Methods {

public:
	Methods(int);
	int C(int, const bvec&);
	int E(const bvec&);
	double F(const bvec&);

	bvec mu_lambda(int mu, int lambda, int iterN);
	bvec one_plus_one(int iterN);

	static bvec random_bvec(int len);
	bvec random_bvec();

	void sbm(bvec&, int);
	void sbm(bvec&);

	void simulating_annealing(const double t, int nb_iterations);
	void init_simulating_annealing(const double t);
	std::vector<bool> get_neighbor();
	void cooling();

private:
	int N;
	int N2;
	double opt;
	bvec S;
	std::binomial_distribution<int> bin_dis;
	std::uniform_int_distribution<int> dis;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
