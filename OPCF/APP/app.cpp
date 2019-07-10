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

	viewmodel->SetModel(model);
	
	// binding properties
	_mainwindow.set_function(model->getFunction());

	//command
	_mainwindow.set_runCommand(viewmodel->getQueryCommand());

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
