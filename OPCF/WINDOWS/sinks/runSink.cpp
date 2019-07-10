#include "../mainwindow.h"
#include "runSink.h"

runSink::runSink(MainWindow* p) throw():ptr_mainwindow(p)
{
}

void runSink::OnCommandComplete(const std::string& str, bool bOK)
{
	ptr_mainwindow->update();
}
