#include <iostream>
#include "methods.h"


int main () {
	
	Methods l(5);
//	std::cout << "E: " << l.E() << '\n';
	l.one_plus_one();
	//don't forget adding 0 at the biggining 
	std::cout << l.F({0, 1,1, 1, 0, 1}) << '\n';
	return 0;
}
