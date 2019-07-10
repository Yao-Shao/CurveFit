#include "../mainwindow.h"
#include "runSink.h"
#include <QtDebug>

runSink::runSink(MainWindow* ptr) {
	ptr_mainwindow = ptr;
}

void runSink::OnPropertyChanged(const std::string& str)
{
#ifndef NDEBUG
	qDebug() << "In windows sink get change signal snd toupdate\n";
#endif // !NDEBUG
	if (str == "Function")
	{
		ptr_mainwindow->update();
	}
}