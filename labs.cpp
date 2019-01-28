#include "labs.h"
#include "labsoptimal.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <iostream>
#include <tuple>
#include <algorithm>
#include <cmath>

int corr(int n, int k, const Bvec &S) {
	int sum = 0;
	for (int i = 0; i < n - k; i++)
		sum += (((S.get(i) xor S.get(i + k)) == 0) ? 1 : -1);
	return sum;
}

int energy(int n, const Bvec &S) {
	int sum = 0;
	for (int k = 1; k < n; k++) {
		int tmp = corr(n, k, S);
		sum += tmp * tmp;
	}
	return sum;
}

double merit (int n, const Bvec &S) {
	return ((double) (n * n)) / (2.0 * ((double) energy(n, S)));
}

Labs::Labs (int n)
	: N(n)
	, optVec(optimalSolution(n))
	, optE(energy(n,this->optVec))
	, optF(merit(n,this->optVec)) { }

int Labs::E(const Bvec &S) { return energy(N, S); }

double Labs::F(const Bvec &S) { return merit(N, S); }
