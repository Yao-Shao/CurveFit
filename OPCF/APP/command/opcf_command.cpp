#include "opcf_command.h"
#include"../app.h"
#include <QtDebug>
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
#ifndef NDEBUG
	qDebug()<< "Int SetParameter:\n";
#endif // !NDEBUG

	(ptrapp->get_viewmodel())->Execc_QueryCommand(std::any_cast<Param_opcf>(param));
}

void opcf_command::Exec()
{
#ifndef NDEBUG
	qDebug() << "ptrapp->getmainwindow().show\n";
#endif // !NDEBUG

	(ptrapp->getmainwindow())->show();
}
