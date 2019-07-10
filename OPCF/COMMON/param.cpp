#include "param.h"


Param_opcf::Param_opcf()
{
}

Param_opcf::Param_opcf(const Param_opcf& src) :P(src.P), t(src.t)
{
}

Param_opcf::Param_opcf(Param_opcf&& src)
{
	P = static_cast<Points&&>(src.P);
	t = static_cast<Type&&>(src.t);
}

Param_opcf& Param_opcf::operator=(const Param_opcf& src)
{
	if (this != &src) {
		P = src.P;
		t = src.t;
	}
	return *this;
}

Param_opcf& Param_opcf::operator=(Param_opcf&& src)
{
	if (this != &src) {
		P = static_cast<Points&&>(src.P);
		t = static_cast<Type&&>(src.t);
	}
	return *this;
}

const Type Param_opcf::get_type() const throw()
{
	return t;
}

void Param_opcf::set_type(const Type tt)
{
	t = tt;
}

Points Param_opcf::get_points() const throw()
{
	return P;
}

void Param_opcf::set_point(Points p_src)
{
	P = p_src;
}
