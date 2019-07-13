#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<math.h>
#include<strstream>
#include<qdebug.h>
#define Infinity 100000000
#define BLOCK_SIZE 400
enum Type {
	LINEAR_FUNCTION,
	QUADRATIC_FUNCTION,
	CUBIC_FUNCTION,
	EXPONENTIAL_FUNCTION,
	LN_FUNCTION,
	NORMAL_FUNCTION
};
struct point_with_range {
	double functions[51][4];
	double points[50];
	int pointnum;
};
class Function
{
private:
	std::string function;
	Type type;
	double power_function[4];    //use by power function
	double other_function[2];    //use by Exponential function or In function,0 place is coefficient
	point_with_range normal_function;//use for normal function
private:
	void add_power(bool pos, double num, int power);
	void add_normalpower(bool pos, double num, int power, int place);
	void add_Exp(bool pos, double num, double com, bool com_pos);
	void add_Ln(bool pos, double num, double com, bool com_pos);
public:
	bool convert();
	Function()throw() {}
	void set_type(Type t) { type = t; }
	Function& operator=(const std::string& s);//you just need to give the function string
	Function& operator=(std::string&& s);
	double get_y(const double& x);
	double get_dy(const double& x);
	std::string get_function() const;
	void set_function(std::string& func);
	void setnum(const int& n) { normal_function.pointnum = n; }
	Type get_type() { return type; };
};
class Point
{
private:
	double x;
	double y;
public:
	Point(double xx = 0.0, double yy = 0.0) throw() : x(xx), y(yy) {}
	Point(const Point& p) : x(p.x), y(p.y) {}
	Point(Point&& p) noexcept;
	bool operator!=(Point& p);
	bool operator<(const Point& p);
	Point& operator=(const Point& p);
	Point& operator=(Point&& p);
	void setx(const double& xx) throw();
	void sety(const double& yy) throw();
	double& getx() throw();
	double& gety() throw();
	void reset(const double& xx, const double& yy) { x = xx; y = yy; }
};
typedef std::vector<Point> Points;
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