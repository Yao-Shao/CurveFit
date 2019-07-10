#pragma once
#include <QtDebug>
#include "..//COMMON/etlbase.h"
#include "..//COMMON/param.h"

class app_opcf;

class opcf_command:public ICommandBase
{
private:
	app_opcf* ptrapp;
public:
	opcf_command(app_opcf* ptr_app);
	~opcf_command();
	void SetParameter(const std::any& param);
	void Exec();
};

