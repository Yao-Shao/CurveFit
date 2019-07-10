#pragma once
#pragma once

#include "../MODEL/Model.h"
#include "../VIEWMODEL/viewmodel.h"
#include "../WINDOWS/DrawGraph.h"
#include "../WINDOWS/mainwindow.h"

class app_opcf
{
private:
	MainWindow _mainwindow;
	std::shared_ptr<Model> model;
	std::shared_ptr<ViewModel> viewmodel;
	//std::shared_ptr<Function> sp_function;

public:
	app_opcf();
	~app_opcf();
	void run();
	MainWindow* getmainwindow();
	std::shared_ptr<ViewModel> get_viewmodel();

};
