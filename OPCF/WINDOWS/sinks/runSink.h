#ifndef RUN_SINK_H
#define RUN_SINK_H

#include "..//COMMON/etlbase.h"
class MainWindow;
class runSink : public IPropertyNotification
{
public:
	runSink(MainWindow* ptr);
	virtual void OnPropertyChanged(const std::string& str);

private:
	MainWindow* ptr_mainwindow;
};


#endif // !RUN_SINK_h
