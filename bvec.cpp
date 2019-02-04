#include "bvec.h"
#include <cstdlib>
#include <iostream>
#include <array>
#include <vector>
#include <cassert>
#include <random>


Bvec::Bvec(int n): b(n) {
	assert (n > 0);
}

Bvec::Bvec(const std::vector<bool> &v): b(v) {
	assert (v.size() > 0);
}

int Bvec::size() const { return b.size(); }

void Bvec::randomise() {
	for (size_t i = 0; i < b.size(); i++) if (rand() % 2 == 0) b[i] = true;
}

void Bvec::clear() { for (uint i = 0; i < b.size(); i++) b[i] = false; }


// Of course these two should be [] operators
bool Bvec::get(int i) const { return b.at(i); }
void Bvec::set(int i,bool f) { b[i] = f; }
void Bvec::flipBit(int i) { b[i] = b[i] xor 1; }

size_t Bvec::hash() const {
	return std::hash<std::vector<bool>>()(b);
}

bool Bvec::operator==(const Bvec &other) const {
	return (this->b) == (other.b);
}

void Bvec::print_encoding() const {
	if(b.size() == 0) return;
	int count = 1;
	bool last_bit = b[0];
	for(size_t i = 1; i < b.size(); ++i) {
		if(b[i] == last_bit) count++;
		else {
			std::cout << count << ' ';
			count = 1;
			last_bit = b[i];
		}
	}

	std::cout << count << '\n';
}

std::basic_ostream<char>& operator << (std::basic_ostream<char>& os, Bvec b) {
	os << "[";
	for(int i = 0; i < b.size(); i++) {
		os << " " << b.get(i);
	}
	os << " ]";
	return os;
}
