#include <iostream>
#include <fstream>
#include "bvec.h"
#include "solver.h"
#include "method.h"

using namespace std;

// Move it elsewhere
void dumpStats(vector<vector<statItem>> plots, string prefix) {
	for (uint i = 0; i < plots.size(); i++) {
		ofstream outS;

		char buffer[50];
		sprintf(buffer, "./plotData/%s%d.csv", prefix.c_str(), i);
		outS.open(buffer);

		auto stats = plots[i];
		for (uint j = 0; j < stats.size(); j++) {
			auto val = stats[j];
			outS << get<0>(val) << ", " << get<1>(val) << ", " << get<2>(val) << "\n";
		}
		outS.close();
	}
}

void compareMuLambdas() {
	Labs l(50);

	vector<vector<statItem>> plots;

	vector<tuple<MuLambda, int>> configs =
		{ make_tuple(MuLambda(l, 2, 5, 0.3), 300)
		, make_tuple(MuLambda(l, 5, 40, 0.3), 300)
		, make_tuple(MuLambda(l, 20, 20, 0.3), 300)
		, make_tuple(MuLambda(l, 30, 30, 0.3), 300)
		};

	for (uint i = 0; i < configs.size(); i++) {
		MuLambda s = get<0>(configs[i]);
		int iters = get<1>(configs[i]);

		s.run(iters);
		cout << "mulambda " << i << ": " << l.F(s.getOptimal()) << '\n';
		plots.push_back(s.getStats());
		s.reset();
	}
	dumpStats(plots,"mulambda");
}

void simpleDemo() {
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


	MuLambda s2(l, 2, 5, 0);

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

	dumpStats(plots,"main");
}

int main () {
	//simpleDemo();
	compareMuLambdas();

	return 0;
}
