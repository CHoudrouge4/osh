#include <iostream>
#include <fstream>
#include "bvec.h"
#include "solver.h"
#include "method.h"

// this is very sad!!
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
			outS << get<0>(val) << "  " << get<1>(val) << "  " << get<2>(val) << "\n";
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
	Labs l(5);

	Bvec v(5);
	v.randomise();
	cout << "Random: " << l.F(v) << " at " << v << '\n';
	cout << "Its optimal: " << l.optF << " with value: " << l.F(l.optF) << '\n';

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


//	SA s3(l, 0.75, 0.5);
//	s3.run(100);
//	s3.set_initial_tempreature(10000);
//	s3.set_cooling_option('e');
//	std::cout << "SA F 100: " << l.F(s3.getOptimal()) << '\n';
//	s3.print_sequence();
//	s3.reset();

	dumpStats(plots,"main");
}

void testing_SA() {
	Labs l(5);

	Bvec v(5);
	v.randomise();
	cout << "Random: " << l.F(v) << " at " << v << '\n';

	vector<vector<statItem>> plots;
//-------------------------------------------------------
	double alpha = 0.20;
	double mu    = 0.10;
	double t0    = 10000;

	SA s3(l, alpha, mu);

	s3.run(100);
	s3.set_initial_tempreature(t0);
	s3.set_cooling_option('l');
	std::cout << "SA F 100: " << l.F(s3.getOptimal()) << '\n';
	s3.print_sequence();
	s3.reset();

	s3.run(500);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 500: " << l.F(s3.getOptimal()) << '\n';
	s3.print_sequence();
	s3.reset();

	s3.run(1000);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 1000: " << l.F(s3.getOptimal()) << '\n';
	s3.print_sequence();
	s3.reset();

	dumpStats(plots,"sa_stat");

}

int main () {
	Labs l(34);
	cout << "Optimal: " << l.optVec << " " << l.optE << " " << l.optF << "\n";

	//simpleDemo();
	//compareMuLambdas();
	//ambdas();
	//testing_SA();


//	Labs l(5);
//	std::vector<bool> v = {0, 0, 0, 1, 0, 1};
//	Bvec b(v);
//	b.print_encoding();
//	std::cout << l.F(b) << '\n';
	return 0;
}
