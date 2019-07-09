#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
#include<strstream>
#define Infinity 100000000
#define BLOCK_SIZE 400
class Function
{
 private:
	std::string function;
	Type type;
	union {
		double power_function[4];    //use by power function
		double other_function[2];    //use by Exponential function or In function,0 place is coefficient
	};
private:
	bool convert();
	void add_power(int pos, int num, int power);
	void add_Exp(int pos, int num, int com);
	void add_In(int pos, int num, int com);
public:
    Function()throw(){}
	Function& operator=(const std::string& s);//you just need to give the function string
	Function& operator=(std::string&& s);
	double get_y(const double& x);
	double get_dy(const double& x);
};
class Point
{
private:
	double x;
	double y;
public:
	Point(double xx = 0.0, double yy = 0.0) throw(): x(xx), y(yy){}
	Point(const Point& p): x(p.x),y(p.y){}
	Point(Point&& p);
	bool operator!=(Point& p);
	Point& operator=(const Point& p);
	Point& operator=(Point&& p);
	void setx(const double& xx) throw();
	void sety(const double& yy) throw();
	double& getx() throw();
	double& gety() throw();
	void reset(const double& xx, const double& yy) { x = xx; y = yy; }
};
typedef std::vector<Point> Points;
enum Type {
	LINEAR_FUNCTION,
	QUADRATIC_FUNCTION,
	EXPONENTIAL_FUNCTION,
	IN_FUNCTION,
	NORMAL_FUNCTION
};
//异常类， 可以用 throw XXXException("XXX error!")调用
class ValueException : public std::exception {
public:
	ValueException(std::string s) :text(s) {}
	std::string what() {
		return text;
	};
private:
	std::string text;
};