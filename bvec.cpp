#include "bvec.h"
#include <cstdlib>
#include <array>
#include <vector>
#include <random>

std::ostream& operator << (std::ostream& os, const Bvec v) {
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

void Bvec::clear() { b.clear(); }


// Of course these two should be [] operators
bool Bvec::get(int i) const { return b.at(i); }
void Bvec::set(int i,bool f) { b[i] = f; }
void Bvec::flipBit(int i) { b[i] = b[i] xor 1; }

//bool& Bvec::getEl(int i) { return b.at(i); }

//const bool Bvec::operator[](int i) const { return b[i]; }
//bool& Bvec::operator[](int i) const { return b.at(i); }
