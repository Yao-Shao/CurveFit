#include "../mainwindow.h"
#include "runSink.h"

runSink::runSink(MainWindow* p) throw():ptr_mainwindow(p)
{
}

void runSink::OnPropertyChanged(const std::string& str)
{
	ptr_mainwindow->update();
}
