#include <iostream>
#include "solver.h"
#include "method.h"

int main () {
	Labs l(100);

	std::cout << "Random: " << l.F(l.random_bvec()) << '\n';

	Method m(l);

	bvec res1 = m.one_plus_one(100);
	std::cout << "1+1 F 10^2: " << l.F(res1) << '\n';

	bvec res2 = m.one_plus_one(1000);
	std::cout << "1+1 F 10^3: " << l.F(res2) << '\n';

	bvec res3 = m.one_plus_one(10000);
	std::cout << "1+1 F 10^4: " << l.F(res3) << '\n';

	bvec res4 = m.one_plus_one(100);
	std::cout << "mu lambda F 10^2: " << l.F(res4) << '\n';

	bvec res5 = m.one_plus_one(1000);
	std::cout << "mu lambda F 10^3: " << l.F(res5) << '\n';

	bvec res6 = m.one_plus_one(10000);
	std::cout << "mu lambda F 10^4: " << l.F(res6) << '\n';

	return 0;
}
