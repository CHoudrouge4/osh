#pragma once
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>


struct Bvec {
	std::vector<bool> b;
	Bvec() {}
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

	bool operator==(const Bvec &other) const;
};

std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, Bvec b);

namespace std {
	template <>
	struct hash<Bvec> {
		size_t operator()(const Bvec& v) const {
			return hash<vector<bool>>()(v.b);
		}
	};
}
