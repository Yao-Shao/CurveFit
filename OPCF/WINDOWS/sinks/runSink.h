#ifndef RUN_SINK_H
#define RUN_SINK_H

#include "..//COMMON/etlbase.h"

class MainWindow;

class runSink : public ICommandNotification
{
public:

	runSink(MainWindow* ptr);
	virtual void OnCommandComplete(const std::string& str, bool bOK);

private:
	MainWindow* ptr_mainwindow;
};

#endif // !MAINWINDOW_SINK_h
