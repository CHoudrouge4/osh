#pragma once
#include <vector>

class Bvec {
private:
	std::vector<bool> b;

public:
	Bvec(int n);

	int size();
	void randomise();
	void clear();

	// Of course these two should be [] operators
	bool get(int i) const;
	void set(int i,bool v);
	void flipBit(int i);
};
