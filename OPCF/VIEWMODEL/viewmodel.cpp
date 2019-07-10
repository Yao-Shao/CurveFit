#include "viewmodel.h"
#include <QtDebug>

ViewModel::ViewModel():m_cmdQuery(std::make_shared<QueryCommand>(this)),
						m_sink(std::make_shared<ViewModelSink>(this))
{
}

void ViewModel::SetModel(const std::shared_ptr<Model>& model)
{
	m_model = model;
#ifndef NDEBUG
	qDebug() << "Add m_sink of viewmodel to model\n";
#endif // !NDEBUG
	m_model->AddPropertyNotification(std::static_pointer_cast<IPropertyNotification>(m_sink));
}

std::shared_ptr<Function> ViewModel::getFunction()
{
	return m_model->getFunction();
}

std::shared_ptr<ICommandBase> ViewModel::getQueryCommand()
{
	return std::static_pointer_cast<ICommandBase>(m_cmdQuery);
}

bool ViewModel::call_model_fit(Param_opcf& p)
{
#ifndef NDEBUG
	qDebug() << "Int send param to model Execc_QueryCommand:\n";
	qDebug() << "Type: " << p.get_type();
	qDebug() << "\n point number" << p.get_points().size();
	qDebug() << "\n";
#endif // !NDEBUG
	bool whether_fit;
	whether_fit = m_model->opcf_fit(p);
	return whether_fit;
}
