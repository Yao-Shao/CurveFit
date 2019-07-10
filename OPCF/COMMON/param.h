#pragma once
#include "base.h"

class  Param_opcf
{
public:
	Param_opcf() throw();
	Param_opcf(const Param_opcf& src);
	Param_opcf(Param_opcf&& src);

	Param_opcf& operator=(const Param_opcf& src);
	Param_opcf& operator=(Param_opcf&& src);

	const Type get_type() const throw();
	void set_type(const Type tt);
	Points get_points() const throw();
	void set_point(Points p_src);

private:
	Points P;
	Type t;
};