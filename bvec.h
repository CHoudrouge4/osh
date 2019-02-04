#pragma once
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>


struct Bvec {
	Bvec(int n);
	Bvec(const std::vector<bool> &v);

	int size() const;
	void randomise();
	void clear();

	// Of course these two should be [] operators
	bool get(int i) const;
	void set(int i,bool v);
	void flipBit(int i);
	size_t hash() const;

	bool operator==(const Bvec &other) const;

	void print_encoding() const;

private:
	std::vector<bool> b;
};

std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, Bvec b);

namespace std {
	template <>
	struct hash<Bvec> {
		size_t operator()(const Bvec& v) const {
			return v.hash(); // hash<vector<bool>>()(v.b);
		}
	};
}
