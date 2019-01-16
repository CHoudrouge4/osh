#include "methods.h"

std::random_device Methods::rd;
std::mt19937 Methods::gen(rd());

template <class T>
std::ostream& operator << (std::ostream& os, const std::vector<T>& v) {
	os << "{ ";
	for(int i = 1; i < v.size(); ++i) {
		os << " " << v[i];
	}
	os << "}";
	return os;
}

Methods::Methods (int n) {
        N  = n;
        N2 = n * n;
        S = std::vector<bool>(n + 1, false);
}

int Methods::C(int k, const  std::vector<bool> &S) {
        int sum = 0;
        for(int i = 1; i <= N - k; ++i) sum += (((S[i] xor S[i + k]) == 0) ? 1 : -1);
        return sum;
}

int Methods::E(const std::vector<bool> &S) {
        int sum = 0;
        for(int k = 1; k <= N - 1; k++) {
                int tmp = C(k, S);
                sum += tmp * tmp;
        }
        return sum;
}

double Methods::F(const std::vector<bool> &S) {
        return N2 / ((double) 2.0 * E(S));
}

void Methods::one_plus_one() {
	std::binomial_distribution<> d(N, 1/((double)N));
	int nb_iterations = 10000;
	for(int i = 0; i < nb_iterations; i++) {
    	int l = d(gen);
    	std::vector<bool> x = S;
        sbm(x, l);
		double fx = F(x);
		double fs = F(S);
		//	std::cout << "fx " << fx << ' ' << "fS " << fs << '\n';
        if(fx > fs) {
			S = x;
			opt = fx;
			//std::cout << "opt: " << opt << '\n';
			//std::cout << S << '\n';
			}
	}
}

void Methods::sbm(std::vector<bool>& x, int l) {
        std::uniform_int_distribution<> dis(1, N);
        for(int i = 0; i < l; ++i) {
                int index = dis(gen);
                x[index] = x[index] xor 1;
        }
}

void Methods::init_simulating_annealing(const double t, const int l) {
	temprature = t;
	sbm(S, l);
}

std::vector<bool> Methods::get_neighbor() {
	std::vector<bool> neighbor = S;
	sbm(neighbor, 1);
	return neighbor;
}

void Methods::simulating_annealing(const double t, int nb_iterations) {
	std::binomial_distribution<> d(N, 1/((double)N));
    std::uniform_real_distribution<> urd(0, 1);
	int l = d(gen);	
	init_simulating_annealing(t, l);
	for(int i = 0; i < nb_iterations; ++i) {
		auto neighbor = get_neighbor();	
		double fs = F(S);
		double fn = F(neighbor);
		if(fn > fs) {
			S = neighbor;
		} else {
			
		}
	}
}
