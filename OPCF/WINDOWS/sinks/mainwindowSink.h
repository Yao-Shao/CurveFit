#ifndef MAINWINDOW_SINK_H
#define MAINWINDOW_SINK_H

#include "..//COMMON/etlbase.h"

class MainWindow;
class mainwindowSink : public IPropertyNotification
{
public:
	mainwindowSink(MainWindow* ptr);
	virtual void OnPropertyChanged(const std::string& str);

private:
	MainWindow* ptr_mainwindow;
};


#endif // !MAINWINDOW_SINK_h
