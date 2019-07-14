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
#include <QEvent>
#include <stack>
#include "../COMMON/base.h"
#include "../COMMON/etlbase.h"
#include "../COMMON/param.h"
#include "sinks/updateSink.h"
#include "sinks/runSink.h"
#define NDEBUG
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

	void update();
	void error_info();
	void run_error(const std::string& str);
	void set_function(std::shared_ptr<Function> spFunction);
	void set_real_points(std::shared_ptr<Points>spRealPoints);
	void set_sample_points(std::shared_ptr<Points>spsamplePoints);
	void set_dy_points(std::shared_ptr<Points>dyPoints);
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
	void loadExcelFile();
	void runActionTrigger();
	void isAddingActionTrigger();
	void clear();


	void showDerivedActionTrigger();
	/* undo redo*/
	void undoTrigger();
	void redoTrigger();

	/*show points*/
	void slotPointHoverd(const QPointF& point, bool state);
	void movePoint(const QPointF& p);

	/*seek help file*/
	void openHelpFile();

private:
	/* draw */
	QLabel* fitTypeLabel;
	QComboBox* fitTypeComboBox;
	QLabel* widthLable;
	QSpinBox* widthSpinBox;
	QToolButton* colorBtn;
	QToolButton* clearBtn;

	/*layout */
	QGridLayout* m_layout;
	void setLayout();

	/*central widget*/
	QWidget* centralWidget;

	/* table chart functionText*/
	QChartView* chartView;
	QGraphicsScene* chartscene;
	QChart* function_view;
	QPlainTextEdit* functionText;
	QTableWidget* table;


	bool initFuncView;
	bool pressAddingBtn;
	QValueAxis* axisX;
	QValueAxis* axisY;
	QScatterSeries* basePoints;


	bool whether_move_point;


	QPixmap myPix;
	QLabel* error_label_pic;

	/*show points*/
	QLabel* m_valueLabel;

	/* param */
	Type fitType;
	Points pointsData;
	Param_opcf m_param;

	std::shared_ptr<Function> spFunction;
	std::shared_ptr<Points> real_xy_points;
	std::shared_ptr<Points> dyPoints;
	std::shared_ptr<Points> sample_points;
	std::shared_ptr<Point> range_x;
	std::shared_ptr<Point> range_y;
	std::shared_ptr<QPixmap> pix;

	std::shared_ptr<updateSink> m_updateSink;
	std::shared_ptr<ICommandBase> m_cmdRun;
	std::shared_ptr<runSink> m_runSink;

	/* file */
	QString openFileAddr;
	QString saveFileAddr;
	QString LastFileName;
	bool FileIsNew;
	bool FileChanged;
	bool flag_isOpen;
	int movepoint_row;

	/*ReDo and UnDo stack*/
	std::stack<Param_opcf> redo_stack;
	std::stack<Param_opcf> undo_stack;
	bool undo_flag;
	bool redo_flag;

	/**/
	QScatterSeries* all_points;
	QLineSeries* series;
	QScatterSeries* samplepoints;
	QScatterSeries* samplepoints_o;

	bool getPoints();
	bool checkPoints();
	void createToolBar();
	void createMenu();
	void createTable();
	void createFuncText();
	void createFuncView();
	void InitFuncView();
	

	void mouseMoveEvent(QMouseEvent* e);
	void mousePressEvent(QMouseEvent* e);
	void mouseReleaseEvent(QMouseEvent* e);
	bool addPoint(QPointF p);
};

#endif // MAINWINDOW_H
