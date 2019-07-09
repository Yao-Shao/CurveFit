#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCloseEvent>
#include <QPixMap>
#include "ui_mainwindow.h"
#include "../COMMON/base.h"
#include "drawgraph.h"

#define LENGTH 1080
#define WIDTH 720
#define ROW 20
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

	std::shared_ptr<command>
	std::shared_ptr<Type> type;
	std::shared_ptr<QPixmap> pix;
	std::shared_ptr<Points> pointsData;
	std::shared_ptr<

	/* file */
	QString openFileAddr;
	QString saveFileAddr;
	
};

#endif // MAINWINDOW_H

