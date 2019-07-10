#include "../mainwindow.h"
#include "runSink.h"

runSink::runSink(MainWindow* ptr) {
	ptr_mainwindow = ptr;
}

void runSink::OnPropertyChanged(const std::string& str)
{
	if (str == "Function")
	{
		ptr_mainwindow->update();
	}
}