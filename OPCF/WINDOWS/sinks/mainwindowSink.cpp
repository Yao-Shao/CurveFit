#include "../mainwindow.h"
#include "mainwindowSink.h"

mainwindowSink::mainwindowSink(MainWindow* p) throw():ptr_mainwindow(p)
{
}

void mainwindowSink::OnPropertyChanged(const std::string& str)
{
	ptr_mainwindow->update();
}
