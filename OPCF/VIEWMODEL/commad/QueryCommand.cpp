#include "QueryCommand.h"

QueryCommand::QueryCommand(ViewModel* p) throw():ptr_viewmodel(p)
{
}

void QueryCommand::SetParameter(const std::any& param)
{
	m_param = std::any_cast<Param>(param);
}

void QueryCommand::Exec()
{

}
