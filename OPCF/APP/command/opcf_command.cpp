#include "opcf_command.h"
#include"../app.h"

opcf_command::opcf_command(app_opcf* ptr_app)
{
	ptrapp = ptr_app;
}

opcf_command::~opcf_command()
{
}

void opcf_command::SetParameter(const std::any& param)
{
}

void opcf_command::Exec()
{
	(ptrapp->getmainwindow())->show();
}
