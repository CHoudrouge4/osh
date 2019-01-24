#include <iostream>
#include "bvec.h"
#include "solver.h"
#include "method.h"

int main () {
	Labs l(50);

	Bvec v(50);
	v.randomise();
	std::cout << "Random: " << l.F(v) << " at " << v << '\n';


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

	return 0;
}
