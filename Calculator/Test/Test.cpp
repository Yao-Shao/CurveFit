#include "../Code/Calculator.h"
#include "../Code/Calculator.cpp"
#include <iostream>

int main()
{
	Calculator c;
	int a, b;
	a = 1;
	b = 2;

	if (c.add(a, b) != (a + b)) {
		std::cout << "Add Error!" << std::endl;
		return -1;
	}
	if (c.sub(a, b) != (a - b)) {
		std::cout << "Sub Eroor!" << std::endl;
		return -2;
	}


	return 0;
}