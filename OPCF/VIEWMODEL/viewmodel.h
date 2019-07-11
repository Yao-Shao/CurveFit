#pragma once
#include "../MODEL/Model.h"
#include "../common/etlbase.h"
#include "command/RunFitCommand.h"
#include "sinks/ViewModelSink.h"

class ViewModel :public Proxy_PropertyNotification<ViewModel>,
	public Proxy_CommandNotification<ViewModel>
{
public:
	ViewModel();
	void SetModel(const std::shared_ptr<Model>& model);
	std::shared_ptr<Function> getFunction();
	std::shared_ptr<Points>getRealPoints();
	std::shared_ptr<Points>getSamplePoints();
	std::shared_ptr<Point>getRangeX();
	std::shared_ptr<Point>getRangeY();
	std::shared_ptr<ICommandBase> get_fitCommand();
	bool call_model_fit(Param_opcf& p);

private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<RunFitCommand> m_cmdFit;
	std::shared_ptr<ViewModelSink> m_sink;
};
