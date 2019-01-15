#pragma once
#include <vector>
#include <random>

class Methods {

public:
	Methods(int);
	int C(int, const std::vector<bool>&);
	int E(const std::vector<bool>&);
	double F(const std::vector<bool>&);

	std::vector<bool> one_plus_one(int iterN);
	void sbm(std::vector<bool>&);

private:
	int N;
	int N2;
	std::vector<bool> S;

	// Random Generation
	static std::random_device rd;
	static std::mt19937 gen;
};
