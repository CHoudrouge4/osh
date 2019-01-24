#pragma once
#include "bvec.h"
#include <vector>
#include <random>
#include <iostream>

class Labs {
public:
	const int N;
	const int N2;

	Labs(int);
	int C(int, const Bvec&);
	int E(const Bvec&);
	double F(const Bvec&);
};
