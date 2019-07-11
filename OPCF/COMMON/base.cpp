#include"base.h"

Function& Function::operator=(const std::string& s)
{
	if (this->function != s) {
		this->function = s;
	}
	convert();
	return *this;
}

Function& Function::operator=(std::string&& s)
{
	if (this->function != s) {
		this->function = static_cast<std::string&&>(s);
	}
	convert();
	return *this;
}

void Function::set_function(std::string& func)
{
	function = func;
}

bool Function::convert()
{
	if (type != NORMAL_FUNCTION) {
		int i = 0;
		double num = 0;
		double com = 0;
		bool com_pos = true;
		bool pos = true; //whether positive
		bool If_Ln = false;
		bool If_Ex = false;
		bool If_power = false;
		bool If_point = false;
		int power = 0;
		int place = 0;
		bool init = true;
		while (function[i] != '\0') {
			if (function[i] <= '9' && function[i] >= '0') {
				if (If_power == 0 && If_Ex == 0 && If_Ln == 0)num = num * 10 + function[i] - '0';
				else if (If_Ex == 1 || If_Ln == 1)com = com * 10 + function[i] - '0';
				else power = function[i] - '0';
				if (If_point == true)place++;
			}
			else if (function[i] == '.')If_point = true;
			else if (function[i] == '-') {
				if (i == 0)pos = false;
				else if (If_Ex == 1 || If_Ln == 1)com_pos = 0;
				else {
					if (init) {
						for (int j = 0; j < 4; j++)power_function[j] = 0;
						init = false;
					}
					add_power(pos, num, power);
					pos = false;
					If_power = false;
					num = power = 0;
				}
			}
			else if (function[i] == '+') {
				if (init) {
					for (int j = 0; j < 4; j++)power_function[j] = 0;
					init = false;
				}
				add_power(pos, num, power);
				pos = true;
				If_power = false;
				num = power = 0;
			}
			else if (function[i] == 'e') {
				If_Ex = true;
				if (If_point == 1) {
					for (int k = 0; k < place; k++)num /= 10.0;
					place = 0;
					If_point = false;
				}
				if (num == 0)num = 1;
			}
			else if (function[i] == 'L') {
				If_Ln = true;
				if (If_point == 1) {
					for (int k = 0; k < place; k++)num /= 10.0;
					place = 0;
					If_point = false;
				}
				if (num == 0)num = 1;
				i++;
			}
			else if (function[i] == 'x' && If_Ex == 0 && If_Ln == 0) {
				If_power = true;
				if (num == 0)num = 1;
				else {
					for (int k = 0; k < place; k++)num /= 10.0;
					place = 0;
					If_point = false;
				}
				power = 1;
			}
			else if (function[i] == 'x' && (If_Ex == 1 || If_Ln == 1)) {
				if (com == 0)com = 1;
				else if (If_point == 1) {
					for (int k = 0; k < place; k++)com /= 10.0;
					If_point = false;
					place = 0;
				}
			}
			i++;
		}
		if (If_Ex == 1) {
			add_Exp(pos, num, com, com_pos);
		}
		else if (If_Ln == 1) {
			add_Ln(pos, num, com, com_pos);
		}
		else {
			if (init) {
				for (int j = 0; j < 4; j++)power_function[j] = 0;
				init = false;
			}
			if (power == 0) {
				if (If_point == true) {
					for (int j = 0; j < place; j++)num /= 10.0;
				}
			}
			add_power(pos, num, power);
		}
	}
	else {
	int i = 0, j = 0;
		int power = 0, place = 0;
		bool pos = true, If_point = false, If_power = false, init = true;
		double num = 0.0;
		normal_function.pointnum = 0;
		while (function[i] != '\0') {
			while (function[i] != '\n' && function[i] != '\0') {
				if (function[i] <= '9' && function[i] >= '0') {
					if (If_power == 0)num = num * 10 + function[i] - '0';
					else power = function[i] - '0';
					if (If_point == true)place++;
				}
				else if (function[i] == '.')If_point = true;
				else if (function[i] == '-') {
					if (i == 0)pos = false;
					else {
						if (init) {
							for (int k = 0; k < 4; k++)normal_function.functions[j][k] = 0;
							init = false;
						}
						add_normalpower(pos, num, power, j);
						pos = false;
						If_power = false;
						num = power = 0;
					}
				}
				else if (function[i] == '+') {
					if (init) {
						for (int j = 0; j < 4; j++)power_function[j] = 0;
						init = false;
					}
					add_normalpower(pos, num, power, j);
					pos = true;
					If_power = false;
					num = power = 0;
				}
				else if (function[i] == 'x') {
					If_power = true;
					if (num == 0)num = 1;
					else {
						for (int k = 0; k < place; k++)num /= 10.0;
						place = 0;
						If_point = false;
					}
					power = 1;
				}
				else if (function[i] == '(') {
					if (init) {
						for (int j = 0; j < 4; j++)power_function[j] = 0;
					}
					if (power == 0) {
						if (If_point == true) {
							for (int j = 0; j < place; j++)num /= 10.0;
						}
					}
					add_normalpower(pos, num, power, j);
					j++;
					init = true;
				}
				else if (function[i] == ',') {
					if (pos == false) num *= -1;
					if (If_point == true) {
						for (int j = 0; j < place; j++)num /= 10.0;
					}
					normal_function.points[normal_function.pointnum++] = num;
					pos = true;
					If_power = false;
					num = power = 0;
				}
				else if (function[i] == ')') {
					if (pos == false) num *= -1;
					if (If_point == true) {
						for (int j = 0; j < place; j++)num /= 10.0;
					}
					if (function[i + 1] == '\0')normal_function.points[normal_function.pointnum] = num;
					pos = true;
					If_power = false;
					num = power = 0;
				}
				i++;
			}
			if (function[i] == '\n')i++;
		}
	}
	return 1;
}
void Function::add_power(bool pos, double num, int power) {
	if (pos == 0)num *= -1;
	power_function[power] = num;
}
void Function::add_normalpower(bool pos, double num, int power, int place)
{
	if (pos == 0)num *= -1;
	normal_function.functions[place][power] = num;
}
void Function::add_Exp(bool pos, double num,double com, bool com_pos)
{
	if (pos == 0)num *= -1;
	if (com_pos == 0)com *= -1;
	other_function[0] = num;
	other_function[1] = com;
}
void Function::add_Ln(bool pos, double num, double com, bool com_pos)
{
	if (pos == 0)num *= -1;
	if (com_pos == 0)com *= -1;
	other_function[0] = num;
	other_function[1] = com;
}
double Function::get_y(const double& x)
{
	if (type == LN_FUNCTION) {
		if (x <= 0)throw ValueException("x couldn't be less or equal to 0 in IN_FUNCTION");
		else return other_function[0] * log(other_function[1] * x);
	}
	else if (type == EXPONENTIAL_FUNCTION) {
		return other_function[0] * exp(other_function[1]*x);
	}
	else if(type == LINEAR_FUNCTION || type == QUADRATIC_FUNCTION){
		return power_function[0] + power_function[1] * x + power_function[2] * x * x + power_function[3] * x * x * x;
	}
	else if (type == NORMAL_FUNCTION) {
		if (x < normal_function.points[0])return normal_function.functions[0][0] + normal_function.functions[0][1] * x + normal_function.functions[0][2] * x * x + normal_function.functions[0][3] * x * x * x;
		else if (x > normal_function.points[normal_function.pointnum - 1])return normal_function.functions[normal_function.pointnum-2][0] + normal_function.functions[normal_function.pointnum-2][1] * x + normal_function.functions[normal_function.pointnum-2][2] * x * x + normal_function.functions[normal_function.pointnum-2][3] * x * x * x;
		else {
			for (int i = 0; i < normal_function.pointnum-1; i++) {
				if (x >= normal_function.points[i] && x <= normal_function.points[i + 1]) {
					return normal_function.functions[i][0] + normal_function.functions[i][1] * x + normal_function.functions[i][2] * x * x + normal_function.functions[i][3] * x * x * x;
				}
			}
		}
	}
}
double Function::get_dy(const double& x)
{
	if (type == LN_FUNCTION) {
		if (x == 0.0)return Infinity;
		else return other_function[0] / x;
	}
	else if (type == EXPONENTIAL_FUNCTION) {
		return other_function[0] * other_function[1] * exp(other_function[1] * x);
	}
	else {
		return power_function[1] + 2 * power_function[2] * x + 3 * power_function[3] * x * x;
	}
}

std::string Function::get_function() const
{
	return function;
}
Point::Point(Point&& p) noexcept
{
	x = static_cast<double&&>(p.x);
	y = static_cast<double&&>(p.y);
}
bool Point::operator!=(Point& p)
{
	if (x == p.getx() && this->y == p.gety())return 0;
	else return 1;
}
Point& Point::operator=(const Point& p)
{
	if (this != &p) {
		x = p.x;
		y = p.y;
	}
	return *this;
}
Point& Point::operator=(Point&& p) 
{
	if (this != &p) {
		x = static_cast<double&&>(p.x);
		y = static_cast<double&&>(p.y);
	}
	return *this;
}
void Point::setx(const double& xx) throw()
{
	x = xx;
}
void Point::sety(const double& yy) throw() 
{
	y = yy;
}
double& Point::getx() throw()
{
	return x;
}
double& Point::gety() throw()
{
	return y;
}