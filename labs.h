#pragma once
#include <vector>
#include <random>
#include <iostream>

using bvec = std::vector<bool>;

bvec random_bvec(int len);

class Labs {
public:
	const int N;
	const int N2;

	Labs(int);
	int C(int, const bvec&);
	int E(const bvec&);
	double F(const bvec&);

	bvec random_bvec();
};
