#include "../mainwindow.h"
#include "updateSink.h"
#include <QtDebug>

updateSink::updateSink(MainWindow* ptr) {
	ptr_mainwindow = ptr;
}

void updateSink::OnPropertyChanged(const std::string& str)
{
#ifndef NDEBUG
	qDebug() << "In windows sink get change signal snd toupdate\n";
#endif // !NDEBUG
	if (str == "Function")
	{
		ptr_mainwindow->update();
	}
}