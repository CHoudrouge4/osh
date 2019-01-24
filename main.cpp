#include <iostream>
#include "solver.h"
#include "method.h"

int main () {
	Labs l(100);

	Bvec v(100);
	v.randomise();
	std::cout << "Random: " << l.F(v) << '\n';

	MuLambda m(l, 2, 5);

	m.run(100);
	std::cout << "1+1 F 100: " << l.F(m.getOptimal()) << '\n';
	m.reset();

	m.run(1000);
	std::cout << "1+1 F 500: " << l.F(m.getOptimal()) << '\n';
	m.reset();

	m.run(1000);
	std::cout << "1+1 F 1000: " << l.F(m.getOptimal()) << '\n';
	m.reset();



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
