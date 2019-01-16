#pragma once
#include <vector>
#include <random> 
#include <iostream>

class Methods {

public:
	Methods(int);
	// todo descriptive names 
	int C(int, const std::vector<bool>&);
	int E(const std::vector<bool>&);
	double F(const std::vector<bool>&);
	void one_plus_one();
// I am not sure about the use of the reference
	void sbm(std::vector<bool>&, int l);

	void simulating_annealing(const double t, int nb_iterations);
	void init_simulating_annealing(const double t, const int l);
	std::vector<bool> get_neighbor();
	void cooling();
private:
	int N;
	int N2;
	std::vector<bool> S;
	double opt;

	double temprature;
// Random Generation 
    static std::random_device rd;
    static std::mt19937 gen;
};
