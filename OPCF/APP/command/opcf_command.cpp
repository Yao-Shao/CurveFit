#include "opcf_command.h"
#include"../app.h"
#include "..//COMMON/param.h"

opcf_command::opcf_command(app_opcf* ptr_app)
{
	ptrapp = ptr_app;
}

opcf_command::~opcf_command()
{
}

void opcf_command::SetParameter(const std::any& param)
{
	ptrapp->get_viewmodel()->Execc_QueryCommand(std::any_cast<Param_opcf>(param));
}

void opcf_command::Exec()
{
	(ptrapp->getmainwindow())->show();
}
