#include <iostream>
#include "Calculator.h"

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
