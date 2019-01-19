#include "labs.h"
#include <cstdlib>
#include <array>
#include <functional>
#include <algorithm>
#include <cmath>

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
	os << "{ ";
	for(int i = 1; i < v.size(); ++i) {
		os << " " << v[i];
	}
	os << "}";
	return os;
}

Labs::Labs (int n): N(n), N2(n*n) { }

int Labs::C(int k, const bvec &S) {
	int sum = 0;
	for (int i = 0; i < N - k; i++)
		sum += (((S[i] xor S[i + k]) == 0) ? 1 : -1);
	return sum;
}

int Labs::E(const bvec &S) {
	int sum = 0;
	for (int k = 0; k < N - 1; k++) {
		int tmp = C(k, S);
		sum += tmp * tmp;
	}
	return sum;
}

double Labs::F(const bvec &S) {
	return N2 / ((double) 2.0 * E(S));
}

bvec random_bvec(int len) {
	bvec x(len);
	for (int i = 0; i < len; i++)
		if (rand() % 2 == 0) x[i] = true;
	return x;
}

bvec Labs::random_bvec() {
	return ::random_bvec(N);
}
