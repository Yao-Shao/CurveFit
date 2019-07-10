#include "QueryCommand.h"
#include "../viewmodel.h"

QueryCommand::QueryCommand(ViewModel* p) throw():ptr_viewmodel(p)
{
}

void QueryCommand::SetParameter(const std::any& param)
{
	m_param = std::any_cast<Param_opcf>(param);
}

void QueryCommand::Exec()
{
	ptr_viewmodel->call_model_fit(m_param);
	ptr_viewmodel->Fire_OnCommandComplete("QueryCommand", true);
}
