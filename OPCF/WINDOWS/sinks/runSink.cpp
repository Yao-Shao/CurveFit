#include "../mainwindow.h"
#include "runSink.h"

runSink::runSink(MainWindow* p) throw():ptr_mainwindow(p)
{
}

void runSink::OnCommandComplete(const std::string& str, bool bOK)
{
	if (str == "RunFitCommand") {
		if (!bOK) {
			ptr_mainwindow->error_info();
		}
	}
}
