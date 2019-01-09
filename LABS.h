#pragma once
#include <vector>
#include <random> 

class LABS {

public:
	LABS(int);
	int C(int, const std::vector<bool>&);
	int E(const std::vector<bool>&);
	double F(const std::vector<bool>&);
	void one_plus_one();
// I am not sure about the use of the reference
	void sbm(std::vector<bool>&, int l);
	


private:
	int N;
	int N2;
	std::vector<bool> S;

// Random Generation 
    static std::random_device rd;
    static std::mt19937 gen;
};
