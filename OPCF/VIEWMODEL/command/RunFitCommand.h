#pragma once


#include "../../COMMON/etlbase.h"
#include "../COMMON/base.h"
#include "../COMMON/param.h"
class ViewModel;

class RunFitCommand:public ICommandBase
{
public:
	RunFitCommand(ViewModel* p) throw();

	virtual void SetParameter(const std::any& param);
	virtual void Exec();


private:
	ViewModel* ptr_viewmodel;
	Param_opcf m_param;
};

