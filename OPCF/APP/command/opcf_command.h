#pragma once

#include "..//COMMON/etlbase.h"


class app_opcf;

class opcf_command:public ICommandBase
{
private:
	app_opcf* ptrapp;
public:
	opcf_command(app_opcf* ptr_app);
	~opcf_command();
	virtual void SetParameter(const std::any& param);
	virtual void Exec();
};

