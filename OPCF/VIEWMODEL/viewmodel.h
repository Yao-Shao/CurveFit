#pragma once


#include "../MODEL/Model.h"
#include "../common/etlbase.h"
#include "commad/QueryCommand.h"
#include "sinks/ViewModelSink.h"

class ViewModel :public Proxy_PropertyNotification<ViewModel>,
	public Proxy_CommandNotification<ViewModel>
{
public:
	ViewModel();
	void SetModel(const std::shared_ptr<Model>& model);
	std::shared_ptr<Function> getFunction();
	std::shared_ptr<ICommandBase> getQueryCommand();
	void Execc_QueryCommand(Param p);

private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<QueryCommand> m_cmdQuery;
	std::shared_ptr<ViewModelSink> m_sink;
};
