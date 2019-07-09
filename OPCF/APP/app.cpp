#include "app.h"

app_opcf::app_opcf()
{
}

app_opcf::~app_opcf()
{
}

void app_opcf::run()
{
	model = std::make_shared<Model>();
	viewmodel = std::make_shared<ViewModel>();
	sp_function = std::make_shared<Function>();

	//½«modelÓëviewmodel°ó¶¨¡£
	viewmodel->SetModel(model);
	_mainwindow.SetViewModel(viewmodel);

	model->set_function(sp_function);
	_mainwindow.set_function(sp_function);

	sp_opcf_command = std::make_shared<opcf_command>(this);

	_mainwindow.set_ptrCommand(std::static_pointer_cast<ICommandBase>(this->sp_opcf_command));
	_mainwindow.show();
}

MainWindow* app_opcf::getmainwindow()
{
	return &_mainwindow;
}

std::shared_ptr<ViewModel> app_opcf::get_viewmodel()
{
	return viewmodel;
}
