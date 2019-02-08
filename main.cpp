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
		cout << "mulambda " << i << ": " << l.F(s.get_opt_vec()) << '\n';
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

	SA s3(l, 0.75, 0.5);
	s3.set_initial_tempreature(10000);
	s3.run(10000);
	std::cout << "SA F 100: " << l.F(s3.get_opt_vec()) << '\n';
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
	std::cout << "SA F 1: " << l.F(s3.get_opt_vec()) << '\n';
	std::cout << "opt seq " <<  s3.get_opt_vec() << '\n';
	s3.reset();



	s3.run(10000);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 2: " << l.F(s3.get_opt_vec()) << '\n';
	s3.reset();

	s3.run(20000);
//	s3.set_initial_tempreature(t0);
//	s3.set_cooling_option('e');
	std::cout << "SA F 3: " << l.F(s3.get_opt_vec()) << '\n';
	s3.reset();

	dumpStats(plots,"sa_stat");

}

void measure_SA(int n, int threads_num) {
	Runner r;

	SA sa(Labs(n), 0.75, 0.5);
	sa.set_initial_tempreature(10000);

	vector<pair<Solver*, long long>> solvers;
	solvers.push_back(make_pair(&sa,20000));

	r.execute(solvers, threads_num, 40, "plotData/measure_sa");
}

void test_TS() {
	Labs l(50);
	TS s(l, 10);
//	s.run(10000);
//	std::cout << "TS F 1: " << l.F(s.get_opt_vec()) << '\n';
//	s.reset();

	s.test_flip_val();
//	dumpStats(plots,"ts_stat");
}

void test_MA() {
	const int n = 41;
	Labs l(n);
	const int popsize = 100;
	double px = 0.8;
	double pm = 0.3;
	const int max_itr = 100;
	MA s(l, popsize, px, pm);
	s.set_max_itr(max_itr);

	s.run(300000);
	s.get_optimums();
	std::cout << "MA F 1: " << l.F(s.get_opt_vec()) << '\n';
	s.reset();

//	dumpStats(plots,"ts_stat");
}

void predict_timeout_sa() {
	int threads_num = 3;
	int sample_size = 40;
	int timeout = 60000;
	int nLo = 15;
	int nHi = 28;
	Runner r;

	vector<pair<Solver*, long long>> solvers;
	for (int n = nLo; n < nHi; n++) {
		SA* sa = new SA(Labs(n), 0.75, 0.5);
		sa->set_initial_tempreature(10000);
		solvers.push_back(make_pair(sa,timeout));
	}

	r.execute(solvers, threads_num, sample_size, "plotData/timeout_estimation_sa");

	for (auto x : solvers) free(get<0>(x));
}

int main () {
	//measure_SA(25);
	//simpleDemo();
	//compareMuLambdas(30);
	//testing_SA();
	//test_TS();
	//measure_SA(25, 2);
	predict_timeout_sa();

	//std::cout << rand() << '\n';
	//std::cout << rand() << '\n';
}
