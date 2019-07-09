#include "Model.h"


Model::Model()
{

	sp_Function = std::make_shared<Function>();

}

std::shared_ptr<Function> Model::getFunction()
{
	return std::shared_ptr<Function>();
}

void Model::opcf_createFunction(Param p)
{
	Type t = p.get_type();
	Points sp_points = p.get_points();
	(*sp_Function).set_type(t);
	if (t == LINEAR_FUNCTION)
	{
		double ave_x, ave_y, sum_xy, sum_qx;
		double a, b;
		int n;
		std::string func;
		ave_x = ave_y = sum_xy = sum_qx = 0.0;
		n = (sp_points).size();
		for (int i = 0; i < n; i++) {
			ave_x += (sp_points)[i].getx();
			ave_y += (sp_points)[i].gety();
			sum_xy += (sp_points)[i].getx() * (sp_points)[i].gety();
			sum_qx += (sp_points)[i].getx() * (sp_points)[i].getx();
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
		(*sp_Function) = func;
	}
	else if (t == QUADRATIC_FUNCTION) {

	}
	else if (t == EXPONENTIAL_FUNCTION) {

	}
	else if (t == LN_FUNCTION) {

	}
	else if (t == NORMAL_FUNCTION) {

	}
	else {

	}
	//告知其它模块，model里面的Function已经改变
	Fire_OnPropertyChanged("Fuction");
}
