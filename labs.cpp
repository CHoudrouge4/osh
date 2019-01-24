#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>

Labs::Labs (int n): N(n), N2(n*n) { }

int Labs::C(int k, const Bvec &S) {
	int sum = 0;
	for (int i = 0; i < N - k; i++)
		sum += (((S.get(i) xor S.get(i + k)) == 0) ? 1 : -1);
	return sum;
}

int Labs::E(const Bvec &S) {
	int sum = 0;
	for (int k = 0; k < N - 1; k++) {
		int tmp = C(k, S);
		sum += tmp * tmp;
	}
	return sum;
}

double Labs::F(const Bvec &S) {
	return N2 / ((double) 2.0 * E(S));
}
