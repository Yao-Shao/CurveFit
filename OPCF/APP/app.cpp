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
	_mainwindow.set_function(viewmodel->getFunction());
	_mainwindow.set_real_points(viewmodel->getRealPoints());
	_mainwindow.set_range_x(viewmodel->getRangeX());
	_mainwindow.set_range_y(viewmodel->getRangeY());
	_mainwindow.set_sample_points(viewmodel->getSamplePoints());
	_mainwindow.set_dy_points(viewmodel->getDyPoints());

	//command
	_mainwindow.set_runCommand(viewmodel->get_fitCommand());

	//notifications
	viewmodel->AddPropertyNotification(_mainwindow.get_updateSink());
	viewmodel->AddCommandNotification(_mainwindow.get_runSink());

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
