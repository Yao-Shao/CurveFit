#include "Model.h"
#include <QtDebug>
#define precision 1e-13

Model::Model()
{

	sp_Function = std::make_shared<Function>();
}

std::shared_ptr<Function> Model::getFunction()
{
	return sp_Function;
}

void Model::set_function(std::shared_ptr<Function> func)
{
	sp_Function = func;
}

void Model::opcf_createFunction(Param_opcf p)
{
	Type t = p.get_type();
	Points sp_points = p.get_points();
	(*sp_Function).set_type(t);
#ifndef NDEBUG
	qDebug() << "Int Create Function\n";
	qDebug() << "Type: " <<t;
	qDebug() << "\n point number" << sp_points.size();
	qDebug() << "\n";
#endif // !NDEBUG
	if (t == LINEAR_FUNCTION)
	{
		double ave_x, ave_y, sum_xy, sum_qx;
		double a, b;
		int n;
		std::string func;
		ave_x = ave_y = sum_xy = sum_qx = 0.0;
		n = sp_points.size();
		for (int i = 0; i < n; i++) {
			ave_x += sp_points[i].getx();
			ave_y += sp_points[i].gety();
			sum_xy += sp_points[i].getx() * sp_points[i].gety();
			sum_qx += sp_points[i].getx() * sp_points[i].getx();
		}
		ave_x /= n;
		ave_y /= n;
		b = (sum_xy - n * ave_x * ave_y) / (sum_qx - n * ave_x * ave_x);
		a = ave_y - b * ave_x;
		if (b != 0) {
			func += std::to_string(b);
			func += "x";
		}
		if (a < 0) {
			func += '-';
			a *= -1;
			func += std::to_string(a);
			func += "\0";
		}
		else if (a > 0) {
			func += '+';
			func += std::to_string(a);
			func += '\0';
		}
		else if (a == 0) func += '\0';
		sp_Function->set_function(func);
	}
	else if (t == QUADRATIC_FUNCTION) {
		int n = sp_points.size();
		double a,b,c,m1,m2,m3,z1,z2,z3;
		double sumx = 0, sumx2 = 0, sumx3 = 0, sumx4 = 0, sumy = 0, sumxy = 0, sumx2y = 0;
		a = b = c = 0;
		for (int i = 0; i < n; i++) {
			sumx += sp_points[i].getx();
			sumx2 += sp_points[i].getx() * sp_points[i].getx();
			sumx3 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
			sumx4 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
			sumy += sp_points[i].gety();
			sumxy += sp_points[i].getx() * sp_points[i].gety();
			sumx2y += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].gety();
		}
		do { 
			m1 = a;a = (sumx2y - sumx3 * b - sumx2 * c) / sumx4; z1 = (a - m1) * (a - m1);
			m2 = b; b = (sumxy - sumx * c - sumx3 * a) / sumx2; z2 = (b - m2) * (b - m2);
			m3 = c; c = (sumy - sumx2 * a - sumx * b) / n; z3 = (c - m3) * (c - m3); 
		} while ((z1 > precision) || (z2 > precision) || (z3 > precision));
		std::string func;
		if (a != 0) {
			func += std::to_string(a);
			func += "x^2";
		}
		if (b != 0) {
			if (b < 0) {
				func += std::to_string(b);
				func += 'x';
			}
			else {
				func += '+';
				func += std::to_string(b);
				func += 'x';
			}
		}
		if (c != 0) {
			if (c < 0)func += std::to_string(c);
			else {
				func += '+';
				func += std::to_string(c);
			}
		}
		func += '\0';
		sp_Function->set_function(func);
	}
	else if (t == EXPONENTIAL_FUNCTION) {
		int n = sp_points.size();
		double a, b, Inb;
		double sumx = 0, sumIny = 0, sumx2 = 0, sumxIny = 0;
		for (int i = 0; i < n; i++) {
			sumx += sp_points[i].getx();
			sumIny += log(sp_points[i].gety());
			sumx2 += sp_points[i].getx() * sp_points[i].getx();
			sumxIny += sp_points[i].getx() * log(sp_points[i].gety());
		}
		a = (n * sumxIny - sumx * sumIny) / (n * sumx2 - sumx * sumx);
		Inb = (sumx2 * sumIny - sumxIny * sumx) / (n * sumx2 - sumx * sumx);
		b = exp(Inb);
		std::string func;
		if (b == 0)func += "0\0";
		else {
			func += std::to_string(b);
			func += 'e';
			if (a == 0)func += '\0';
			else {
				func += '^';
				func += std::to_string(a);
				func += "x\0";
			}
		}
		sp_Function->set_function(func);
	}
	else if (t == LN_FUNCTION) {
		int n = sp_points.size();
		double Ina, a, b;
		double sumyInx = 0, sumy = 0, sumInx = 0, sumInx2 = 0;
		std::string func;
		for (int i = 0; i < n; i++) {
			sumyInx += sp_points[i].gety() * log(sp_points[i].getx());
			sumy += sp_points[i].gety();
			sumInx += log(sp_points[i].getx());
			sumInx2 += log(sp_points[i].getx()) * log(sp_points[i].getx());
		}
		b = (n * sumyInx - sumy * sumInx) / (n * sumInx2 - sumInx * sumInx);
		Ina = (sumy * sumInx - b * sumInx * sumInx) / (n * b * sumInx);
		a = exp(Ina);
		func += std::to_string(b);
		func += "Ln";
		func += std::to_string(a);
		func += "x\0";
		sp_Function->set_function(func);


	}
	else if (t == NORMAL_FUNCTION) {

	}
	else {

	}
#ifndef NDEBUG
	qDebug() << "End of opcf and the function is" << QString::fromStdString((*sp_Function).get_function()) << "\n";
	qDebug() <<"Fire_OnPropertyChanged(Function) \n";
#endif // !NDEBUG
	//告知其它模块，model里面的Function已经改变
	Fire_OnPropertyChanged("Function");
}
