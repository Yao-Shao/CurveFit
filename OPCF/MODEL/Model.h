#pragma once

#include "../COMMON/base.h"
#include <memory>
#include "../COMMON/etlbase.h"
#include "../COMMON/param.h"
#define POINTSNUMBER 500
struct Mux_Points {
	double x[50], y[50];
};
class Model : public Proxy_PropertyNotification<Model>
{
public:
	Model();
	~Model() {}
	std::shared_ptr<Function> getFunction();
	std::shared_ptr<Points>getRealPoints();
	std::shared_ptr<Point>getRangeX();
	std::shared_ptr<Point>getRangeY();
	/*拟合函数*/
	bool opcf_fit(Param_opcf& p);
	/*得到对应的点,传入拟合函数种类作为参数，以选择x轴范围*/
	bool get_realXYPoints(Type t);
	/*映射为作图的点*/
	bool get_mappedXYPoints();
	/*搜索点集，得到最大最小值，方便作图*/
	double get_min_real_x();
	double get_max_real_x();
	double get_min_real_y();
	double get_max_real_y();
	double get_min_sample_x();
	double get_max_sample_x();


private:
	std::shared_ptr<Function>sp_Function;
	std::shared_ptr<Points>samplePoints;
	std::shared_ptr<Points>real_xy_points;
	std::shared_ptr<Points>map_to_img_xy;
	std::shared_ptr<Point>range_x;
	std::shared_ptr<Point>range_y;
private:
	void sort(Mux_Points& m, const int& n);
};
