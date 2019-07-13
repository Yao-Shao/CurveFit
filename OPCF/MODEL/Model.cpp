#include <QtDebug>
#include "Model.h"
#include "math.h"
#define precision 1e-13
#define STARTLNFUNCT 1e-4

Model::Model()
{
	sp_Function = std::make_shared<Function>();
	samplePoints = std::make_shared<Points>();
	real_xy_points = std::make_shared<Points>();
	dxy_points = std::make_shared<Points>();
	range_x = std::make_shared<Point>();
	range_y = std::make_shared<Point>();
}
std::shared_ptr<Function> Model::getFunction()
{
	return sp_Function;
}

std::shared_ptr<Points> Model::getRealPoints()
{
	return real_xy_points;
}

std::shared_ptr<Points> Model::getDyPoints()
{
	return dxy_points;
}

std::shared_ptr<Points> Model::getSamplePoints()
{
	return samplePoints;
}

std::shared_ptr<Point> Model::getRangeX()
{
	return range_x;
}

std::shared_ptr<Point> Model::getRangeY()
{
	return range_y;
}

bool Model::opcf_fit(Param_opcf& p)
{

	Type t = p.get_type();
	Points sp_points = p.get_points();
	(*sp_Function).set_type(t);


	/*send sample points to model*/
	samplePoints->clear();
	for (auto i = 0; i < sp_points.size(); i++)
	{
		Point point = sp_points[i];
		samplePoints->push_back(point);
	}


#ifndef NDEBUG
	qDebug() << "Int Create Function\n";
	qDebug() << "Type: " << t;
	qDebug() << "\n point number" << sp_points.size();
	qDebug() << "\n";
	qDebug() << "Test if pass to model" << samplePoints->size();
#endif // !NDEBUG

	/*fuction fit*/
	std::string propertychanged = "Function";
	/*no sample point , run orror*/
	if (sp_points.size() == 0)
	{
		propertychanged = "NoSamplePoints";
		Fire_OnPropertyChanged(propertychanged);
		return true;
	}
	else if (sp_points.size() == 1) {
		std::string func;
		func += std::to_string(sp_points[0].gety());
		func += '\0';
		(*sp_Function).set_type(LINEAR_FUNCTION);
		sp_Function->set_function(func);
	}
	else {
		if (t == LINEAR_FUNCTION)
		{
			double ave_x, ave_y, sum_xy, sum_qx;
			double a, b;
			int n;
			bool init = true;
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
				init = false;
				func += std::to_string(b);
				func += "x";
			}
			if (a < 0) {
				func += std::to_string(a);
				func += "\0";
			}
			else if (a > 0) {
				if (init == 0)func += '+';
				func += std::to_string(a);
				func += '\0';
			}
			else if (a == 0) func += '\0';
			sp_Function->set_function(func);
		}
		else if (t == QUADRATIC_FUNCTION) {
			int n = sp_points.size();
			double a, b, c, m1, m2, m3, z1, z2, z3;
			double sumx = 0, sumx2 = 0, sumx3 = 0, sumx4 = 0, sumy = 0, sumxy = 0, sumx2y = 0;
			bool init = true;
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
				m1 = a; a = (sumx2y - sumx3 * b - sumx2 * c) / sumx4; z1 = (a - m1) * (a - m1);
				m2 = b; b = (sumxy - sumx * c - sumx3 * a) / sumx2; z2 = (b - m2) * (b - m2);
				m3 = c; c = (sumy - sumx2 * a - sumx * b) / n; z3 = (c - m3) * (c - m3);
			} while ((z1 > precision) || (z2 > precision) || (z3 > precision));
			std::string func;
			if (a != 0) {
				func += std::to_string(a);
				func += "x^2";
				init = false;
			}
			if (b != 0) {
				if (b < 0) {
					func += std::to_string(b);
					func += 'x';
				}
				else {
					if (init == 0)func += '+';
					func += std::to_string(b);
					func += 'x';
				}
				init = false;
			}
			if (c != 0) {
				if (c < 0)func += std::to_string(c);
				else {
					if (init == 0)func += '+';
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
				if (sp_points[i].gety() <= 0) {
					propertychanged = "Expwithwrongy";
					Fire_OnPropertyChanged(propertychanged);
					return true;
				}
			}
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
				if (sp_points[i].getx() <= 0) {
					propertychanged = "Lnwithwrongx";
					Fire_OnPropertyChanged(propertychanged);
					return true;
				}
			}
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
		else if (t == CUBIC_FUNCTION) {
			int n = sp_points.size();
			double sumx = 0.0, sumx2 = 0.0, sumx3 = 0.0, sumx4 = 0.0, sumx5 = 0.0, sumx6 = 0.0, sumy = 0.0, sumxy = 0.0, sumx2y = 0.0, sumx3y = 0.0;
			for (int i = 0; i < n; i++) {
				sumx += sp_points[i].getx();
				sumx2 += sp_points[i].getx() * sp_points[i].getx();
				sumx3 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
				sumx4 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
				sumx5 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
				sumx6 += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx();
				sumy += sp_points[i].gety();
				sumxy += sp_points[i].getx() * sp_points[i].gety();
				sumx2y += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].gety();
				sumx3y += sp_points[i].getx() * sp_points[i].getx() * sp_points[i].getx() * sp_points[i].gety();
			}
			double m[4][5] = { sumx3,sumx2,sumx,n,sumy,
							   sumx4,sumx3,sumx2,sumx,sumxy,
							   sumx5,sumx4,sumx3,sumx2,sumx2y,
							   sumx6,sumx5,sumx4,sumx3,sumx3y };
			double temp[5];
			int p, place = -1;
			double judge;
			double x[4];
			for (int i = 0; i < 3; i++) {
				if (m[i][i] != 0) {
					place = i;
					judge = m[i][i];
				}
				for (p = i; p < 4; p++) {
					if (m[p][i] != 0) {
						if (m[p][i] < judge) {
							place = p;
							judge = m[p][i];
						}
					}
				}
				if (place == -1) {
					propertychanged = "Can'thavesolution";
					Fire_OnPropertyChanged(propertychanged);
					return true;
				}
				if (p != i) {
					for (int k = 0; k < 5; k++) {
						temp[k] = m[p][k];
					}
					for (int k = 0; k < 5; k++) {
						m[p][k] = m[i][k];
					}
					for (int k = 0; k < 5; k++) {
						m[i][k] = temp[k];
					}
				}
				for (int j = i + 1; j < 4; j++) {
					judge = m[j][i] / m[i][i];
					for (int k = 0; k < 5; k++) {
						m[j][k] = m[j][k] - judge * m[i][k];
					}
				}
				place = -1;
			}
			if (m[3][3] == 0) {
				propertychanged = "Can'thavesolution";
				Fire_OnPropertyChanged(propertychanged);
				return true;
			}
			x[3] = m[3][4] / m[3][3];
			for (int i = 2; i >= 0; i--) {
				judge = 0.0;
				for (int j = i + 1; j < 4; j++)judge += m[i][j] * x[j];
				x[i] = (m[i][4] - judge) / m[i][i];
			}
			std::string func;
			bool init = true;
			if (x[0] != 0) {
				func += std::to_string(x[0]);
				func += "x^3";
				init = false;
			}
			if (x[1] != 0) {
				if (x[1] < 0) {
					func += std::to_string(x[1]);
				}
				else if (x[1] > 0) {
					if (init == 0)func += '+';
					func += std::to_string(x[1]);
				}
				func += "x^2";
				init = false;
			}
			if (x[2] != 0) {
				if (x[2] < 0) {
					func += std::to_string(x[2]);
				}
				else if (x[2] > 0) {
					if (init == 0)func += '+';
					func += std::to_string(x[2]);
				}
				func += 'x';
				init = false;
			}
			if (x[3] != 0) {
				if (x[3] < 0) {
					func += std::to_string(x[3]);
				}
				else if (x[3] > 0) {
					if (init == 0)func += '+';
					func += std::to_string(x[3]);
				}
			}
			func += '\0';
			sp_Function->set_function(func);
		}
		else if (t == NORMAL_FUNCTION) {
			int n = sp_points.size();
			Mux_Points mux_point;
			double a[51], b[51], c[51], d[51], h[50], afa[50], L[51], mu[51], z[51];
			for (int i = 0; i < n; i++) {
				mux_point.x[i] = sp_points[i].getx();
				mux_point.y[i] = sp_points[i].gety();
			}
			sort(mux_point, n);
			for (int i = 0; i < n; i++)a[i] = mux_point.y[i];
			for (int i = 0; i < n - 1; i++)h[i] = mux_point.x[i + 1] - mux_point.x[i];
			for (int i = 1; i < n - 1; i++)afa[i] = 3 / h[i] * (a[i + 1] - a[i]) - 3 / h[i - 1] * (a[i] - a[i - 1]);
			L[0] = 1.0; mu[0] = 0.0; z[0] = 0.0;
			for (int i = 1; i < n - 1; i++) {
				L[i] = 2 * (mux_point.x[i + 1] - mux_point.x[i - 1]) - h[i - 1] * mu[i - 1];
				mu[i] = h[i] / L[i];
				z[i] = (afa[i] - h[i - 1] * z[i - 1]) / L[i];
			}
			L[n - 1] = 1.0; z[n - 1] = 0.0; c[n - 1] = 0.0;
			for (int j = n - 2; j >= 0; j--) {
				c[j] = z[j] - mu[j] * c[j + 1];
				b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
				d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
			}
			for (int j = 0; j < n - 1; j++) {
				double tempa = a[j], tempb = b[j], tempc = c[j];
				c[j] = tempc - 3 * d[j] * mux_point.x[j];
				b[j] = 3 * d[j] * mux_point.x[j] * mux_point.x[j] - 2 * tempc * mux_point.x[j] + tempb;
				a[j] = tempa - tempb * mux_point.x[j] + tempc * mux_point.x[j] * mux_point.x[j] - d[j] * mux_point.x[j] * mux_point.x[j] * mux_point.x[j];
			}
			std::string func;
			for (int i = 0; i < n - 1; i++) {
				bool init = true;
				if (d[i] != 0.0) {
					func += std::to_string(d[i]);
					func += "x^3";
					init = false;
				}
				if (c[i] != 0.0) {
					if (c[i] < 0.0) {
						func += std::to_string(c[i]);
						func += "x^2";
					}
					else if (c[i] > 0.0) {
						if (init == 0)func += '+';
						func += std::to_string(c[i]);
						func += "x^2";
					}
					init = false;
				}
				if (b[i] != 0.0) {
					if (b[i] < 0.0) {
						func += std::to_string(b[i]);
						func += 'x';
					}
					else if (b[i] > 0.0) {
						if (init == 0)func += '+';
						func += std::to_string(b[i]);
						func += 'x';
					}
					init = false;
				}
				if (a[i] != 0.0) {
					if (a[i] < 0.0)func += std::to_string(a[i]);
					else if (a[i] > 0.0) {
						if (init == 0)func += '+';
						func += std::to_string(a[i]);
					}
				}
				func += '(';
				func += std::to_string(mux_point.x[i]);
				func += ',';
				func += std::to_string(mux_point.x[i + 1]);
				func += ')';
				if (i != n - 2)func += '\n';
				else func += '\0';
				sp_Function->setnum(n);
				sp_Function->set_function(func);
			}
		}
		sp_Function->convert();
	}

	/*get xy*/
	bool whether_get_real_points;
	whether_get_real_points = get_realXYPoints(t);
	range_x->setx((this->get_min_real_x()));
	range_x->sety((this->get_max_real_x()));
	range_y->setx((this->get_min_real_y()));
	range_y->sety((this->get_max_real_y()));

	/*map to x y in img*/

#ifndef NDEBUG
	qDebug() << "End of opcf_fit and the function is" << QString::fromStdString((*sp_Function).get_function()) << "\n";
	qDebug() << "And we have " << real_xy_points->size() << " points to be painted\n";
	qDebug() << "Fire_OnPropertyChanged(Function) \n";
#endif // !NDEBUG
	Fire_OnPropertyChanged(propertychanged);
	return true;
}

bool Model::get_realXYPoints(Type t)
{
	double x;
	double y;
	double dy;
	double start_x;
	double end_x;
	double length;
	double step;
	start_x = get_min_sample_x();
	end_x = get_max_sample_x();
	length = end_x - start_x;
#ifndef NDEBUG
	qDebug() << "In get_realXYPoints(Type):\n" << "X Range of sample Points  " << start_x << "-" << end_x << "\n";
	qDebug() << "Function: " << QString::fromStdString(sp_Function->get_function());
#endif // !NDEBUG
	switch (t)
	{
	case LINEAR_FUNCTION: {
		start_x = start_x - length / 3;
		end_x = end_x + length / 3;
		start_x = floor(start_x);
		end_x = ceil(end_x);
		step = (end_x - start_x) / POINTSNUMBER;
	}
						  break;
	case QUADRATIC_FUNCTION: {
		start_x = start_x - length / 3;
		end_x = end_x + length / 3;
		start_x = floor(start_x);
		end_x = ceil(end_x);
		step = (end_x - start_x) / POINTSNUMBER;
	}
							 break;
	case CUBIC_FUNCTION: {
		start_x = start_x - length / 3;
		end_x = end_x + length / 3;
		start_x = floor(start_x);
		end_x = ceil(end_x);
		step = (end_x - start_x) / POINTSNUMBER;
	}
							 break;
	case EXPONENTIAL_FUNCTION: {
		start_x = start_x - length / 3;
		end_x = end_x + length / 3;
		start_x = floor(start_x);
		end_x = ceil(end_x);
		step = (end_x - start_x) / POINTSNUMBER;
	}
							   break;
	case LN_FUNCTION: {
		start_x = start_x - length / 3;
		end_x = end_x + length / 3;
		if (start_x <= 0) start_x = STARTLNFUNCT;
		end_x = ceil(end_x);
		step = (end_x - start_x) / POINTSNUMBER;
	}
					  break;
	case NORMAL_FUNCTION: {
		step = (end_x - start_x) / POINTSNUMBER;
	}
						  break;
	default: {
		step = (end_x - start_x) / POINTSNUMBER;
	}
			 break;
	}

	real_xy_points->clear();
	dxy_points->clear();
	if (t == LN_FUNCTION) {
		x = start_x;
		y = sp_Function->get_y(x);
		dy = sp_Function->get_dy(x);
		step = 0.01;
		for (int i = 0; i < 100; i++) {
			Point t;
			Point dt;
			t.setx(x);
			t.sety(y);
			dt.setx(x);
			dt.sety(dy);
			real_xy_points->push_back(t);
			dxy_points->push_back(dt);
			x += step;
			y = sp_Function->get_y(x);
			dy = sp_Function->get_dy(x);
		}
		step = (end_x - start_x - 1) / (POINTSNUMBER - 100);
		for (int i = 100; i < POINTSNUMBER; i++) {
			Point t;
			Point dt;
			t.setx(x);
			t.sety(y);
			dt.setx(x);
			dt.sety(dy);
			real_xy_points->push_back(t);
			dxy_points->push_back(dt);
			x += step;
			y = sp_Function->get_y(x);
			dy = sp_Function->get_dy(x);
		}
	}
	else {
		x = start_x;
		y = sp_Function->get_y(x);
		dy = sp_Function->get_dy(x);
		for (int i = 0; i < POINTSNUMBER; i++) {
			Point t;
			Point dt;
			t.setx(x);
			t.sety(y);
			dt.setx(x);
			dt.sety(dy);
			real_xy_points->push_back(t);
			dxy_points->push_back(dt);
			x += step;
			y = sp_Function->get_y(x);
			dy = sp_Function->get_dy(x);
		}
	}
	return true;
}

bool Model::get_mappedXYPoints()
{
	return false;
}

double Model::get_min_real_x()
{
	double min = ((*real_xy_points)[0]).getx();
	for (auto i = 1; i < real_xy_points->size(); i++) {
		if (min > ((*real_xy_points)[i]).getx())
		{
			min = ((*real_xy_points)[i]).getx();
		}
	}
	return min;
}

double Model::get_max_real_x()
{
	double max = ((*real_xy_points)[0]).getx();
	for (auto i = 1; i < real_xy_points->size(); i++) {
		if (max < ((*real_xy_points)[i]).getx())
		{
			max = ((*real_xy_points)[i]).getx();
		}
	}
	return max;
}

double Model::get_min_real_y()
{
#ifndef NDEBUG
	qDebug() << "In get_min_real_y()\n";
#endif // !NDEBUG
	double min = ((*real_xy_points)[0]).gety();
	for (auto i = 1; i < real_xy_points->size(); i++) {
		if (min > ((*real_xy_points)[i]).gety())
		{
			min = ((*real_xy_points)[i]).gety();
		}
	}
#ifndef NDEBUG
	qDebug() << "min" << min << "\n";
#endif // !NDEBUG
	return min;
}

double Model::get_max_real_y()
{
	double max = ((*real_xy_points)[0]).gety();
	for (auto i = 1; i < real_xy_points->size(); i++) {
		if (max < ((*real_xy_points)[i]).gety())
		{
			max = ((*real_xy_points)[i]).gety();
		}
	}
	return max;
}

double Model::get_min_sample_x()
{
	double min = (*samplePoints)[0].getx();
	for (auto i = 1; i < samplePoints->size(); i++) {
		if (min > (*samplePoints)[i].getx())
		{
			min = (*samplePoints)[i].getx();
		}
	}
	return min;
}

double Model::get_max_sample_x()
{
	double max = (*samplePoints)[0].getx();
	for (auto i = 1; i < samplePoints->size(); i++) {
		if (max < (*samplePoints)[i].getx())
		{
			max = (*samplePoints)[i].getx();
		}
	}
	return max;
}

void Model::sort(Mux_Points & m, const int& n)
{
	int i, j;
	double tempx, tempy;
	for (i = 0; i < n; i++) {
		tempx = m.x[i];
		tempy = m.y[i];
		for (j = i; j > 0; j--) {
			if (m.x[j - 1] > tempx) {
				m.x[j] = m.x[j - 1];
			}
			else break;
		}
		m.x[j] = tempx;
		m.y[j] = tempy;
	}
}