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
#include "ui_mainwindow.h"
#include "../COMMON/base.h"
#include "drawgraph.h"
#include "../COMMON/etlbase.h"
#include "../COMMON/param.h"

#define LENGTH 1080
#define WIDTH 720
#define ROW 50
#define COLUMN 2

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	void set_ptrCommand(std::shared_ptr<ICommandBase> ptrCommand);
	~MainWindow();
	void createToolBar();
	void createMenu();
	void createTable();
	void createFuncText();
	void update();
	void set_function(std::shared_ptr<Function> spFunction);

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



	QLabel* funcBox;
	QTableWidget* table;
	Type fitType;
	QString funcText;
	Param_opcf m_param;

	std::shared_ptr<Function> spFunction;
	std::shared_ptr<QPixmap> pix;
	std::shared_ptr<Points> pointsData;

	/* file */
	QString openFileAddr;
	QString saveFileAddr;
	
	/*½Ó¿Ú*/
	std::shared_ptr<ICommandBase> _ptrCommand;

	void getPoints();
};

#endif // MAINWINDOW_H

