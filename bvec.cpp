#include "bvec.h"
#include <cstdlib>
#include <array>
#include <vector>
#include <random>

std::ostream& operator << (std::ostream& os, const Bvec v) {
	// TODO
	//	os << "{ ";
	//	for(int i = 1; i < v.size(); ++i) {
	//		os << " " << v.get(i);
	//	}
	//	os << "}";
	return os;
}

Bvec::Bvec(int n) {
	b = std::vector<bool>(n);
}

int Bvec::size() { return b.size(); }

void Bvec::randomise() {
	for (uint i = 0; i < b.size(); i++) if (rand() % 2 == 0) b[i] = true;
}

void Bvec::clear() { for (uint i = 0; i < b.size(); i++) b[i] = false; }


// Of course these two should be [] operators
bool Bvec::get(int i) const { return b.at(i); }
void Bvec::set(int i,bool f) { b[i] = f; }
void Bvec::flipBit(int i) { b[i] = b[i] xor 1; }
