#include <iostream>
#include <fstream>
#include "bvec.h"
#include "solver.h"
#include "method.h"

using namespace std;

// Move it elsewhere
void dumpStats(vector<vector<statItem>> plots) {
	for (uint i = 0; i < plots.size(); i++) {
		ofstream outS;

		char buffer[50];
		sprintf(buffer, "./plotData/%d.csv", i);
		outS.open(buffer);

		auto stats = plots[i];
		for (uint j = 0; j < stats.size(); j++) {
			auto val = stats[j];
			outS << get<0>(val) << ", " << get<1>(val) << ", " << get<2>(val) << "\n";
		}
		outS.close();
	}
}

int main () {
	Labs l(50);

	Bvec v(50);
	v.randomise();
	cout << "Random: " << l.F(v) << " at " << v << '\n';

	vector<vector<statItem>> plots;

	OnePlusOne s1(l);

	s1.run(100);
	cout << "1+1 F 100: " << l.F(s1.getOptimal()) << '\n';
	plots.push_back(s1.getStats());
	s1.reset();

	s1.run(1000);
	cout << "1+1 F 500: " << l.F(s1.getOptimal()) << '\n';
	plots.push_back(s1.getStats());
	s1.reset();

	s1.run(1000);
	cout << "1+1 F 1000: " << l.F(s1.getOptimal()) << '\n';
	plots.push_back(s1.getStats());
	s1.reset();


	MuLambda s2(l, 2, 5);

	s2.run(100);
	cout << "mulambda F 100: " << l.F(s2.getOptimal()) << '\n';
	plots.push_back(s2.getStats());
	s2.reset();

	s2.run(1000);
	cout << "mulambda F 500: " << l.F(s2.getOptimal()) << '\n';
	plots.push_back(s2.getStats());
	s2.reset();

	s2.run(1000);
	cout << "mulambda F 1000: " << l.F(s2.getOptimal()) << '\n';
	plots.push_back(s2.getStats());
	s2.reset();

	dumpStats(plots);

	return 0;
}
