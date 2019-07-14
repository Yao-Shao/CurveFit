#include "test.h"
#include <iostream>

int main() {
	bool right;
	right = ViewModel_Test();
	if (right) {
		std::cout << "The program works fine.\n";
	}
	else
	{
		std::cout << "The program doesn;t work.\n";
	}
	return 0;
}