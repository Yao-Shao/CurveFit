#include "RunFitCommand.h"
#include "../viewmodel.h"

RunFitCommand::RunFitCommand(ViewModel* p) throw():ptr_viewmodel(p)
{
}

void RunFitCommand::SetParameter(const std::any& param)
{
	m_param = std::any_cast<Param_opcf>(param);
}

void RunFitCommand::Exec()
{
	bool b;
	b = ptr_viewmodel->call_model_fit(m_param);
	ptr_viewmodel->Fire_OnCommandComplete("RunFitCommand", b);
}
