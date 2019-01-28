#pragma once
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>


class Bvec {
private:
	std::vector<bool> b;

public:
	Bvec(int n);
	Bvec(std::vector<bool> &v);

	int size();
	void randomise();
	void clear();

	// Of course these two should be [] operators
	bool get(int i) const;
	void set(int i,bool v);
	void flipBit(int i);
	void print_encoding() const;
};

std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, Bvec b);
