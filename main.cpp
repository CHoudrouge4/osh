#include <iostream>
#include "solver.h"
#include "method.h"

int main () {
	Labs l(100);

	Bvec v(100);
	v.randomise();
	std::cout << "Random: " << l.F(v) << '\n';


	OnePlusOne s1(l);

	s1.run(100);
	std::cout << "1+1 F 100: " << l.F(s1.getOptimal()) << '\n';
	s1.reset();

	s1.run(1000);
	std::cout << "1+1 F 500: " << l.F(s1.getOptimal()) << '\n';
	s1.reset();

	s1.run(1000);
	std::cout << "1+1 F 1000: " << l.F(s1.getOptimal()) << '\n';
	s1.reset();


	MuLambda s2(l, 2, 5);

	s2.run(100);
	std::cout << "mulambda F 100: " << l.F(s2.getOptimal()) << '\n';
	s2.reset();

	s2.run(1000);
	std::cout << "mulambda F 500: " << l.F(s2.getOptimal()) << '\n';
	s2.reset();

	s2.run(1000);
	std::cout << "mulambda F 1000: " << l.F(s2.getOptimal()) << '\n';
	s2.reset();



	//	Bvec res2 = m.one_plus_one(1000);
	//	std::cout << "1+1 F 10^3: " << l.F(res2) << '\n';
	//
	//	Bvec res3 = m.one_plus_one(10000);
	//	std::cout << "1+1 F 10^4: " << l.F(res3) << '\n';
	//
	//	Bvec res4 = m.one_plus_one(100);
	//	std::cout << "mu lambda F 10^2: " << l.F(res4) << '\n';
	//
	//	Bvec res5 = m.one_plus_one(1000);
	//	std::cout << "mu lambda F 10^3: " << l.F(res5) << '\n';
	//
	//	Bvec res6 = m.one_plus_one(10000);
	//	std::cout << "mu lambda F 10^4: " << l.F(res6) << '\n';

	return 0;
}
