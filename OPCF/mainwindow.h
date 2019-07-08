#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolButton>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QCloseEvent>
#include "ui_mainwindow.h"

#define LENGTH 1080
#define WIDTH 720

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	~MainWindow();
	void createToolBar();
	void createMenu();
	void createTable();

	Ui::MainWindow* ui;

	enum fitType {
		LINE,
		QUAD,
		LOG,
		EXP,
		FREE
	};

protected:
	void closeEvent(QCloseEvent*);

public slots:

	void showStyle();
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
	
	/* file */
	QString openFileAddr;
	QString saveFileAddr;
	
};

#endif // MAINWINDOW_H

