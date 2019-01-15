#include "methods.h"

std::random_device Methods::rd;
std::mt19937 Methods::gen(rd());

Methods::Methods (int n) {
	N  = n;
	N2 = n * n;
	S = std::vector<bool>(N + 1, false);
}

int Methods::C(int k, const std::vector<bool> &S) {
	int sum = 0;
	for (int i = 1; i <= N - k; i++)
		sum += (((S[i] xor S[i + k]) == 0) ? 1 : -1);
	return sum;
}

int Methods::E(const std::vector<bool> &S) {
	int sum = 0;
	for (int k = 1; k <= N - 1; k++) {
		int tmp = C(k, S);
		sum += tmp * tmp;
	}
	return sum;
}

double Methods::F(const std::vector<bool> &S) {
	return N2 / ((double) 2.0 * E(S));
}

std::vector<bool> Methods::one_plus_one(int iterN) {
	std::vector<bool> x = S;
	for (int i = 0; i < iterN; i++) {
		std::vector<bool> y = x;
		sbm(y);
		if(F(y) > F(x)) x = y;
	}
	return x;
}

void Methods::sbm(std::vector<bool>& x) {
	std::uniform_int_distribution<> dis(1, N);
	for (int i = 0; i < x.size(); i++) {
		int guess = dis(gen);
		if (guess == 1) x[i] = x[i] xor 1;
	}
}
