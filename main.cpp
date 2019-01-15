#include <iostream>
#include "methods.h"

int main () {
	Methods l(100);

	std::vector<bool> res1 = l.one_plus_one(100);
	std::cout << "1+1 F 10^2: " << l.F(res1) << '\n';

	std::vector<bool> res2 = l.one_plus_one(1000);
	std::cout << "1+1 F 10^3: " << l.F(res2) << '\n';

	std::vector<bool> res3 = l.one_plus_one(10000);
	std::cout << "1+1 F 10^4: " << l.F(res3) << '\n';

	return 0;
}
