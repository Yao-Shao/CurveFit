#include "app.h"
#include <QtDebug>
app_opcf::app_opcf()
{
}

app_opcf::~app_opcf()
{
}

void app_opcf::run()
{

#ifndef NDEBUG
	qDebug() << "In app_opcf.run()\n";
#endif // !NDEBUG

	//objects
	model = std::make_shared<Model>();
	viewmodel = std::make_shared<ViewModel>();
	sp_function = std::make_shared<Function>();

	viewmodel->SetModel(model);
	_mainwindow.SetViewModel(viewmodel);

	//binding properties
	model->set_function(sp_function);
	_mainwindow.set_function(sp_function);

	//command
	sp_opcf_command = std::make_shared<opcf_command>(this);
	_mainwindow.set_ptrCommand(std::static_pointer_cast<ICommandBase>(this->sp_opcf_command));

	//notifications
	viewmodel->AddPropertyNotification(_mainwindow.get_updateSink());

	
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
