#pragma once
#include "base.h"

class  Param
{
public:
	Param() throw();
	Param(const Param& src);
	Param(Param&& src);

	Param& operator=(const Param& src);
	Param& operator=(Param && src);

	const Type get_type() const throw();
	void set_type(const Type tt);
	Points get_points() const throw();
	void set_point(Points p_src);

private:
	Points P;
	Type t;
};

 Param:: Param()
{
}

 inline Param::Param(const Param& src):P(src.P),t(src.t)
 {
 }

 inline Param::Param(Param&& src)
 {
	 P = static_cast<Points&&>(src.P);
	 t = static_cast<Type&&>(src.t);
 }

 inline Param& Param::operator=(const Param& src)
 {
	 if (this != &src) {
		 P = src.P;
		 t = src.t;
	 }
	 return *this;
 }

 inline Param& Param::operator=(Param&& src)
 {
	 if (this != &src) {
		 P = static_cast<Points&&>(src.P);
		 t = static_cast<Type&&>(src.t);
	 }
	 return *this;
 }

 inline const Type Param::get_type() const throw()
 {
	 return t;
 }

 inline void Param::set_type(const Type tt)
 {
	 t = tt;
 }

 inline Points Param::get_points() const throw()
 {
	 return P;
 }

 inline void Param::set_point(Points p_src)
 {
	 P = p_src;
 }
