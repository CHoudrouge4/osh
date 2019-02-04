#include <iostream>
#include <fstream>
#include "bvec.h"
#include "solver.h"
#include "runner.h"
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
			outS << get<0>(val) << "  " << get<1>(val) << "  " << get<2>(val) << "\n";
		}
		outS.close();
	}
}

void compareMuLambdas(int n) {
	Labs l(n);
	cout << "L = " << n << ", F = " << l.optF << "\n";

	vector<vector<statItem>> plots;

	vector<tuple<MuLambda, int>> configs =
		{ make_tuple(MuLambda(l, 50, 100, 0.6),   20000)
		, make_tuple(MuLambda(l, 50, 100, 0.9),   20000)
		, make_tuple(MuLambda(l, 50, 100, 0.95),  20000)
		, make_tuple(MuLambda(l, 100, 200, 0.99), 20000)
		};

	for (uint i = 0; i < configs.size(); i++) {
		MuLambda s = get<0>(configs[i]);
		int iters = get<1>(configs[i]);

		s.run(iters);
		cout << "mulambda " << i << ": " << l.F(s.getOptimal()) << '\n';
		plots.push_back(s.stats);
		s.reset();
	}
	dumpStats(plots,"mulambda");
}

void simpleDemo() {
	Labs l(25);

	Bvec v(25);
	v.randomise();
	cout << "Random: " << l.F(v) << " at " << v << '\n';
	cout << "Optimal: " << l.optF << " at " << l.optVec << "\n";

	vector<vector<statItem>> plots;

	OnePlusOne s1(l);

	for (int i = 0; i < 3; i++) {
		s1.run(5000);
		cout << "1+1: " << l.F(s1.getOptimal()) << '\n';
		plots.push_back(s1.stats);
		s1.reset();
	}

	MuLambda s2(l, 2, 5, 0.9);

	for (int i = 0; i < 3; i++) {
		s2.run(5000);
		cout << "mulambda F: " << l.F(s2.getOptimal()) << '\n';
		plots.push_back(s2.stats);
		s2.reset();
	}

	SA s3(l, 0.75, 0.5);
	s3.set_initial_tempreature(10000);
	s3.set_cooling_option(false);
	s3.run(10000);
	std::cout << "SA F 100: " << l.F(s3.getOptimal()) << '\n';
	plots.push_back(s3.stats);
	s3.reset();

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

	s3.run(10000);
	s3.set_initial_tempreature(t0);
	s3.set_cooling_option('l');
	std::cout << "SA F 1: " << l.F(s3.getOptimal()) << '\n';
	std::cout << "opt seq " <<  s3.getOptimal() << '\n';
	s3.reset();



	s3.run(10000);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 2: " << l.F(s3.getOptimal()) << '\n';
	s3.reset();

	s3.run(20000);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 3: " << l.F(s3.getOptimal()) << '\n';
	s3.reset();

	dumpStats(plots,"sa_stat");

}

void measure_SA(int n, int threads_num) {
	Runner r;

	vector<SA> sas;
	for (int i = 0; i < threads_num; i++) {
		SA sa(Labs(n), 0.75, 0.5);
		sa.set_initial_tempreature(10000);
		sa.set_cooling_option(false);

		sas.push_back(sa);
	}

	vector<Solver*> solvers(threads_num);
	for (int i = 0; i < threads_num; i++) solvers[i] = &sas[i];

	r.execute(solvers, 30, 10000);

	cout << "Hits: " << r.hits_n <<
		", hits ratio: " << r.hits_ratio <<
		", avg t: " << r.average_t <<
		"\n";
	dumpStats(r.stats, "sa_runner");
}

void test_TS() {
	Labs l(11);
	TS s(l, 10);
	s.run(10);
	std::cout << "TS F 1: " << l.F(s.getOptimal()) << '\n';
	s.reset();

//	dumpStats(plots,"ts_stat");
}

int main () {
	//measure_SA(25);
	//simpleDemo();
	//compareMuLambdas(30);
	//testing_SA();
	//test_TS();
	//measure_SA(23, 2);
	std::cout << rand() << '\n';
	std::cout << rand() << '\n';
}
