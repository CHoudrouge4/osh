#include <iostream>
#include <fstream>
#include "bvec.h"
#include "solver.h"
#include "runner.h"
#include "method.h"

using namespace std;

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

void choose_SA_params() {
	int n = 22;
	int threads_num = 1;
	int sample_size = 20;

	Labs l(n);
	vector<SA> solvers =
		{ SA(l, 0.9, 100)
		, SA(l, 0.9, 500)
		, SA(l, 0.9, 1000)
		, SA(l, 0.9, 5000)
		, SA(l, 0.9, 10000)
		, SA(l, 0.9, 20000)
		, SA(l, 0.5, 100)
		, SA(l, 0.5, 500)
		, SA(l, 0.5, 1000)
		, SA(l, 0.5, 5000)
		, SA(l, 0.5, 10000)
		, SA(l, 0.5, 20000)
		, SA(l, 0.1, 100)
		, SA(l, 0.1, 500)
		, SA(l, 0.1, 1000)
		, SA(l, 0.1, 5000)
		, SA(l, 0.1, 10000)
		, SA(l, 0.1, 20000)
		};

	Runner r;
	vector<pair<Solver*, long long>> config;
	for (uint i = 0; i < solvers.size(); i++) {
		config.push_back(make_pair(&solvers[i],20000));
	}

	r.execute(config, threads_num, sample_size, "plotData/sa_param");
}

void simpleDemo() {
	Labs l(25);

	Bvec v(25);
	v.randomise();
	cout << "Random: " << l.F(v) << " at " << v << '\n';
	cout << "Optimal: " << l.optF << " at " << l.opt_vec << "\n";

	vector<vector<statItem>> plots;

	OnePlusOne s1(l);

	for (int i = 0; i < 3; i++) {
		s1.run(5000);
		cout << "1+1: " << l.F(s1.get_opt_vec()) << '\n';
		plots.push_back(s1.stats);
		s1.reset();
	}

	MuLambda s2(l, 2, 5, 0.9);

	for (int i = 0; i < 3; i++) {
		s2.run(5000);
		cout << "mulambda F: " << l.F(s2.get_opt_vec()) << '\n';
		plots.push_back(s2.stats);
		s2.reset();
	}

	SA s3(l, 0.5, 10000);
	s3.run(10000);
	std::cout << "SA F 100: " << l.F(s3.get_opt_vec()) << '\n';
	plots.push_back(s3.stats);
	s3.reset();

	dumpStats(plots,"main");
}


void measure_SA(int n, int threads_num) {
	Runner r;

	SA sa(Labs(n), 0.5, 10000);

	vector<pair<Solver*, long long>> solvers;
	solvers.push_back(make_pair(&sa,20000));

	r.execute(solvers, threads_num, 40, "plotData/measure_sa");
}

void test_TS() {
	const int n = 25;
	const int timeout = 20000;
	const int threads_num = 3;
	const int sample_size = 40;

	Labs l(n);
	TS ts(l);

	Runner r;
	vector<pair<Solver*, long long>> solvers;
	solvers.push_back(make_pair(&ts, timeout));

	r.execute(solvers, threads_num, sample_size, "plotData/ts_demo_run");

	// 9087 sec with 2N
}

void test_MA() {
	const int n = 25;
	const int timeout = 20000;
	const int threads_num = 3;
	const int sample_size = 20;

	Labs l(n);
	cout << "Optimum: " << l.optF << "\n";
	MA ma(l);

	Runner r;
	vector<pair<Solver*, long long>> solvers;
	solvers.push_back(make_pair(&ma, timeout));

	r.execute(solvers, threads_num, sample_size, "plotData/ma_demo_run");
}

void big_experiment() {
	// Should run for 9.375 hours at most?
	int threads_num = 8;
	int sample_size = 50;
	int timeout = 90000;
	int nLo = 15;
	int nHi = 55;
	Runner r;

	vector<pair<Solver*, long long>> solvers;
	for (int n = nLo; n <= nHi; n++) {
		Labs l(n);

		//Solver* mul = new MuLambda(l,10,150,0.9);
		//solvers.push_back(make_pair(mul,timeout));
		//
		//Solver* opo = new OnePlusOne(l);
		//solvers.push_back(make_pair(opo,timeout));

		Solver* sa = new SA(l, 0.15, 15000);
		solvers.push_back(make_pair(sa,timeout));

		Solver* ma = new MA(l);
		solvers.push_back(make_pair(ma,timeout));

		Solver* ts = new TS(l);
		solvers.push_back(make_pair(ts,timeout));

		Solver* sals = new SALS(l);
		solvers.push_back(make_pair(sals,timeout));
	}

	r.execute(solvers, threads_num, sample_size, "plotData/ts_sals_15-40");

	for (auto x : solvers) free(get<0>(x));
}

void test_speed() {
	int threads_num = 8;
	int sample_size = 30;
	int timeout = 180000;
	int nLo = 50;
	int nHi = 51;
	Runner r;

	vector<pair<Solver*, long long>> solvers;
	for (int n = nLo; n <= nHi; n++) {
		Labs l(n);

		Solver* sa = new SA(l, 0.15, 15000);
		solvers.push_back(make_pair(sa,timeout));

		Solver* ma = new MA(l);
		solvers.push_back(make_pair(ma,timeout));

		Solver* ts = new TS(l);
		solvers.push_back(make_pair(ts,timeout));

		Solver* sals = new SALS(l);
		solvers.push_back(make_pair(sals,timeout));
	}

	r.execute(solvers, threads_num, sample_size, "plotData/speed_comparison");

	for (auto x : solvers) free(get<0>(x));
}

int main () {
	//measure_SA(25);
	//simpleDemo();
	//testing_SA();
	//test_TS();
	//measure_SA(25, 2);
	//test_MA();
	//choose_SA_params();

	//big_experiment();
	test_speed();
}
