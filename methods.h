#pragma once
#include <vector>
#include <random>


using bvec = std::vector<bool>;

class Methods {

public:
	Methods(int);
	int C(int, const bvec&);
	int E(const bvec&);
	double F(const bvec&);

	bvec mu_lambda(int mu, int lambda, int iterN);
	bvec one_plus_one(int iterN);

	static bvec random_vect(int len);
	static void sbm(bvec&);

private:
	int N;
	int N2;
	bvec S;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
