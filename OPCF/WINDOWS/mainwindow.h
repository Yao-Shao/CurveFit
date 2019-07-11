#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCloseEvent>
#include <QPixMap>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QtCharts>
#include "../COMMON/base.h"
#include "drawgraph.h"
#include "../COMMON/etlbase.h"
#include "../COMMON/param.h"
#include "sinks/updateSink.h"
#include "sinks/runSink.h"

#define LENGTH 1080
#define WIDTH 720
#define ROW 50
#define COLUMN 2

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();

	void createToolBar();
	void createMenu();
	void createTable();
	void createFuncText();
	void createFuncView();

	void update(bool bOK);
	void set_function(std::shared_ptr<Function> spFunction);
	void set_real_points(std::shared_ptr<Points>spRealPoints);
	void set_range_x(std::shared_ptr<Point>range_xx);
	void set_range_y(std::shared_ptr<Point>range_yy);

	std::shared_ptr<IPropertyNotification> get_updateSink();
	void set_runCommand(const std::shared_ptr<ICommandBase>& cmd);
	std::shared_ptr<ICommandNotification> get_runSink();



protected:
	void closeEvent(QCloseEvent*);



public slots:
	void showType();
	void showColor();
	bool saveData();
	bool saveGraph();
	bool saveAs();
	void openFile();
	void drawLineActionTrigger();
	void runActionTrigger();
	void drawEclipseActionTrigger();
	void drawRectangleActionTrigger();
	void drawTriangleActionTrigger();

	/* undo redo*/
	void undoTrigger();
	void redoTrigger();
	

private:
	/* draw */
	QLabel* styleLabel;
	QComboBox* styleComboBox;
	QLabel* widthLable;
	QSpinBox* widthSpinBox;
	QToolButton* colorBtn;
	QToolButton* clearBtn;
	QPlainTextEdit* functionText;
	
	/*chart*/
	
	QChartView* chartview;
	QChart* function_view;

	QTableWidget* table;
	Type fitType;
	Param_opcf m_param;

	std::shared_ptr<Function> spFunction;
	std::shared_ptr<Points> real_xy_points;
	std::shared_ptr<Point> range_x;
	std::shared_ptr<Point> range_y;
	std::shared_ptr<QPixmap> pix;
	Points pointsData;

	std::shared_ptr<updateSink> m_updateSink;
	std::shared_ptr<ICommandBase> m_cmdRun;
	std::shared_ptr<runSink> m_runSink;

	/* file */
	QString openFileAddr;
	QString saveFileAddr;

	void getPoints();
};

#endif // MAINWINDOW_H

