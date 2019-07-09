#include "viewmodel.h"

ViewModel::ViewModel():m_cmdQuery(std::make_shared<QueryCommand>(this)),
						m_sink(std::make_shared<ViewModelSink>(this))
{
}

void ViewModel::SetModel(const std::shared_ptr<Model>& model)
{
	m_model = model;
	m_model->AddPropertyNotification(std::static_pointer_cast<IPropertyNotification>(m_sink));
}

std::shared_ptr<Function> ViewModel::getFunction()
{
	return std::shared_ptr<Function>();
}

std::shared_ptr<ICommandBase> ViewModel::getQueryCommand()
{
	return std::shared_ptr<ICommandBase>();
}

void ViewModel::Execc_QueryCommand(Param p)
{
}
