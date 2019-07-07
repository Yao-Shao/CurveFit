#include <iostream>
#include "Calculator.h"


int main()
{
	Calculator c;
    std::cout << "add 1 and 2 " << c.add(1,2) << std::endl; 
	std::cout << "sub 1 and 2 " << c.sub(1, 2);
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单


int Calculator::add(int num1, int num2)
{
	return num1 + num2;
}

int Calculator::sub(int num1, int num2)
{
	return num1 - num2;  // 错误，应该是 num1 - num2
}

Calculator::Calculator()
{
}

Calculator::~Calculator()
{
}
