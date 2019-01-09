#include "LABS.h"

std::random_device LABS::rd;
std::mt19937 LABS::gen(rd());

LABS::LABS (int n) {
	N  = n;
	N2 = n * n;
	S = std::vector<bool>(n + 1, false);
}

int LABS::C(int k, const  std::vector<bool> &S) {
	int sum = 0;
	for(int i = 1; i <= N - k; ++i) sum += (((S[i] xor S[i + k]) == 0) ? 1 : -1); 
	return sum;
}

int LABS::E(const std::vector<bool> &S) {
	int sum = 0;
	for(int k = 1; k <= N - 1; k++) {
		int tmp = C(k, S);
		sum += tmp * tmp;	
	}
	return sum;
}

double LABS::F(const std::vector<bool> &S) {
	return N2 / ((double) 2.0 * E(S));
} 

void LABS::one_plus_one() {
	std::binomial_distribution<> d(N, 1/((double)N));
   	int nb_iterations = 100;
	for(int i = 0; i < nb_iterations; i++) {
	    int l = d(gen);
		std::vector<bool> x = S;
		sbm(x, l);
		if(F(x) > F(S)) S = x;

	}
}

void LABS::sbm(std::vector<bool>& x, int l) {
	std::uniform_int_distribution<> dis(1, N);
	for(int i = 0; i < l; ++i) 	{
		int index = dis(gen);
		x[index] = x[index] xor 1; 
	}
} 
