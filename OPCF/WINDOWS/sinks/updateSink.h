#ifndef UPDATE_SINK_H
#define UPDATE_SINK_H

#include "..//COMMON/etlbase.h"

class MainWindow;

class updateSink : public IPropertyNotification
{
public:
	updateSink(MainWindow* ptr);
	virtual void OnPropertyChanged(const std::string& str);

private:
	MainWindow* ptr_mainwindow;
};


#endif // !UPDATE_SINK_h
