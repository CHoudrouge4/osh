#include "bvec.h"
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>
#include <random>


Bvec::Bvec(int n) {
	b = std::vector<bool>(n);
}

int Bvec::size() { return b.size(); }

void Bvec::randomise() {
	for (size_t i = 0; i < b.size(); i++) if (rand() % 2 == 0) b[i] = true;
}

void Bvec::clear() { for (uint i = 0; i < b.size(); i++) b[i] = false; }


// Of course these two should be [] operators
bool Bvec::get(int i) const { return b.at(i); }
void Bvec::set(int i,bool f) { b[i] = f; }
void Bvec::flipBit(int i) { b[i] = b[i] xor 1; }


std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, Bvec b) {
	os << "[";
	for(int i = 1; i < b.size(); ++i) {
		os << " " << b.get(i);
	}
	os << "]";
	return os;
}
