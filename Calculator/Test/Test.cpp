#include "../Code/Calculator.h"
#include "../Code/Calculator.cpp"
#include <iostream>

int main()
{
	Calculator c;
	int a, b;
	a = 1;
	b = 2;


	std::cout << "Modified by zzh in 2019/7/7" << std::endl;
	std::cout << "菜鸟2号走了一圈" << std::endl;

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