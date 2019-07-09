#pragma once


#include "../../COMMON/etlbase.h"
#include "../viewmodel.h"
#include "../COMMON/base.h"
#include "../COMMON/param.h"
class ViewModel;

class QueryCommand:public ICommandBase
{
public:
	QueryCommand(ViewModel* p) throw();

	virtual void SetParameter(const std::any& param);
	virtual void Exec();
private:
	ViewModel* ptr_viewmodel;

	Param m_param;
};
