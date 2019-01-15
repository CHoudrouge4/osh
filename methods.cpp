#include "methods.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>

std::random_device Methods::rd;
std::mt19937 Methods::gen(rd());

Methods::Methods (int n) {
	N  = n;
	N2 = n * n;
	S = bvec(N + 1, false);
}

int Methods::C(int k, const bvec &S) {
	int sum = 0;
	for (int i = 1; i <= N - k; i++)
		sum += (((S[i] xor S[i + k]) == 0) ? 1 : -1);
	return sum;
}

int Methods::E(const bvec &S) {
	int sum = 0;
	for (int k = 1; k <= N - 1; k++) {
		int tmp = C(k, S);
		sum += tmp * tmp;
	}
	return sum;
}

double Methods::F(const bvec &S) {
	return N2 / ((double) 2.0 * E(S));
}

bvec Methods::random_vect(int len) {
	bvec x(len);
	for (int i = 0; i < len; i++)
		if (rand() % 2 == 0) x[i] = true;
	return x;
}

bvec Methods::one_plus_one(int iterN) {
	bvec x = random_vect(N+1);
	for (int i = 0; i < iterN; i++) {
		bvec y = x;
		sbm(y);
		if(F(y) > F(x)) x = y;
	}
	return x;
}

bvec Methods::mu_lambda(int mu, int lambda, int iterN) {
	std::uniform_int_distribution<> dis(0, mu-1);

	// population
	std::vector<bvec> xs(mu);
	for (int i = 0; i < mu; i++) xs[i] = random_vect(N);

	std::vector<bvec> ys(mu);

	for (int i = 0; i < iterN; i++) {
		// variation
		for (int j = 0; j < lambda; j++) {
			ys[j] = xs[dis(gen)];
			sbm(ys[j]);
		}
		// selection (sort descending)
		std::sort(ys.begin(), ys.end(), [this](bvec a, bvec b) { return F(a) < F(b); });
		for (int j = 0; j < mu; j++) xs[j] = ys[j];
	}

	auto maxValue =
		std::max_element(xs.begin(), xs.end(),
						 [this](bvec a, bvec b) { return F(a) > F(b); });
	return *maxValue;
}

void Methods::sbm(bvec& x) {
	std::uniform_int_distribution<> dis(1, x.size());
	for (int i = 0; i < x.size(); i++) {
		int guess = dis(gen);
		if (guess == 1) x[i] = x[i] xor 1;
	}
}
