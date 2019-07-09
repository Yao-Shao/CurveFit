#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent)
//	ui(new Ui::MainWindow)
{


	QWidget* centralWidget = new QWidget;
	setCentralWidget(centralWidget);
	setMinimumSize(LENGTH, WIDTH);

	setWindowTitle("OPCF");
	setWindowIcon(QIcon(":/OPCF/img/logo.png"));

	drawGraph = new DrawGraph;
	createMenu();
	createToolBar();
	createTable();
}

MainWindow::~MainWindow()
{
	//delete ui;
}

void MainWindow::createMenu()
{
	/* File */
	QMenuBar* pmenuBar = menuBar();
	QMenu* fileMenu = pmenuBar->addMenu("File");

	/*
	QAction* openAction = new QAction("Open");
	openAction->setShortcut((Qt::CTRL | Qt::Key_O));
	fileMenu->addAction(openAction);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
	*/

	QAction * saveDataAction = new QAction("Save data");
	saveDataAction->setShortcut((Qt::CTRL | Qt::Key_S));
	fileMenu->addAction(saveDataAction);
	connect(saveDataAction, SIGNAL(triggered()), this, SLOT(saveData()));

	QAction* saveGraphAction = new QAction("Save graph");
	saveGraphAction->setShortcut((Qt::CTRL | Qt::ALT | Qt::Key_S));
	fileMenu->addAction(saveGraphAction);
	connect(saveGraphAction, SIGNAL(triggered()), this, SLOT(saveGraph()));

	QAction * saveAsAction = new QAction("Save as");
	saveAsAction->setShortcut((Qt::CTRL | Qt::SHIFT | Qt::Key_S));
	fileMenu->addAction(saveAsAction);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	/* Edit */
	QMenu* editMenu = pmenuBar->addMenu("Edit");

	QAction * redoAction = new QAction("Redo");
	redoAction->setShortcut((Qt::CTRL | Qt::Key_R));
	editMenu->addAction(redoAction);
	connect(redoAction, SIGNAL(triggered()), this, SLOT(redoTrigger()));

	QAction * undoAction = new QAction("Undo");
	undoAction->setShortcut((Qt::CTRL | Qt::Key_Z));
	editMenu->addAction(undoAction);
	connect(undoAction, SIGNAL(triggered()), this, SLOT(undoTrigger()));

	QMenu* helpMenu = pmenuBar->addMenu("Help");

}

void MainWindow::createToolBar()
{
	QToolBar* toolBar = addToolBar("Tool");             /*add tool bar object*/

	/* Runing */
	QAction* runAction = new QAction("Run", toolBar);
	runAction->setIcon(QIcon(":/OPCF/img/running.png"));
	runAction->setToolTip(tr("Draw fitted curve"));
	toolBar->addAction(runAction);
	connect(runAction, SIGNAL(clicked()), this, SLOT(runActionTrigger()));

	/*
	QAction* drawLineAction = new QAction("Line", toolBar);
	drawLineAction->setIcon(QIcon(":/src/Line.png"));
	drawLineAction->setToolTip(tr("Line"));
	drawLineAction->setCheckable(true);
	graghGroup->addAction(drawLineAction);
	toolBar->addAction(drawLineAction);
	connect(drawLineAction, SIGNAL(triggered()), this, SLOT(drawLineActionTrigger()));

	QAction* drawEclipseAction = new QAction("Eclipse", toolBar);
	drawEclipseAction->setIcon(QIcon(":/src/Eclipse.png"));
	drawEclipseAction->setToolTip(tr("Eclipse(ctrl for circle)"));
	drawEclipseAction->setCheckable(true);
	graghGroup->addAction(drawEclipseAction);
	toolBar->addAction(drawEclipseAction);
	connect(drawEclipseAction, SIGNAL(triggered()), this, SLOT(drawEclipseActionTrigger()));

	QAction* drawRectangleAction = new QAction("Rectangle", toolBar);
	drawRectangleAction->setIcon(QIcon(":/src/Rectangle.png"));
	drawRectangleAction->setToolTip(tr("Rectangle(ctrl for square)"));
	drawRectangleAction->setCheckable(true);
	graghGroup->addAction(drawRectangleAction);
	toolBar->addAction(drawRectangleAction);
	connect(drawRectangleAction, SIGNAL(triggered()), this, SLOT(drawRectangleActionTrigger()));

	QAction* drawTriangleAction = new QAction("Triangle", toolBar);
	drawTriangleAction->setIcon(QIcon(":/src/Triangle.png"));
	drawTriangleAction->setToolTip(tr("Triangle"));
	drawTriangleAction->setCheckable(true);
	graghGroup->addAction(drawTriangleAction);
	toolBar->addAction(drawTriangleAction);
	connect(drawTriangleAction, SIGNAL(triggered()), this, SLOT(drawTriangleActionTrigger()));
	*/

	/* fit type */
	styleLabel = new QLabel(tr("Fit Type: "));
	styleComboBox = new QComboBox;
	styleComboBox->addItem(tr("Line"), static_cast<int>(LINEAR_FUNCTION));
	styleComboBox->addItem(tr("Quad"), static_cast<int>(QUADRATIC_FUNCTION));
	styleComboBox->addItem(tr("Log"), static_cast<int>(LN_FUNCTION));
	styleComboBox->addItem(tr("Exponential"), static_cast<int>(EXPONENTIAL_FUNCTION));
	styleComboBox->addItem(tr("Free"), static_cast<int>(NORMAL_FUNCTION));

	connect(styleComboBox, SIGNAL(activated(int)), this, SLOT(setType()));
	toolBar->addWidget(styleLabel);
	toolBar->addWidget(styleComboBox);
	
	

	/* Spin box
	widthLable = new QLabel(tr("Line width: "));
	widthSpinBox = new QSpinBox;
	connect(widthSpinBox, SIGNAL(valueChanged(int)), drawWidget, SLOT(setWidth(int)));
	toolBar->addWidget(widthSpinBox);
	toolBar->addWidget(widthLable);
	*/


	/* Color */
	colorBtn = new QToolButton;
	QPixmap pixmap(20, 20);
	pixmap.fill(Qt::black);
	colorBtn->setIcon(QIcon(pixmap));
	connect(colorBtn, SIGNAL(clicked()), this, SLOT(showColor()));
	toolBar->addWidget(colorBtn);

	/* Run 
	clearBtn = new QToolButton;
	clearBtn->setText(tr("Clear"));
	connect(clearBtn, SIGNAL(clicked()), drawWidget, SLOT(clear()));
	toolBar->addWidget(clearBtn);
	*/
}

void MainWindow::createTable()
{
	QTableWidget *table = new QTableWidget(ROW, COLUMN, this);
	//set position
	table->setGeometry(10, 100, 400, 600);


	//set column title
	QStringList columnHeaders;
	columnHeaders << "x" << "y";
	table->setHorizontalHeaderLabels(columnHeaders);

	//set row title
	QStringList rowHeaders;
	for (int i = 0; i < ROW; i++) {

		rowHeaders << (static_cast<QString>("P") + QString::number(i + 1));
	}
	table->setVerticalHeaderLabels(rowHeaders);

	table->setSelectionBehavior(QAbstractItemView::SelectItems);
	table->setEditTriggers(QAbstractItemView::DoubleClicked);

	/* init
	QTableWidgetItem* item0 = new QTableWidgetItem;
	QTableWidgetItem* item1 = new QTableWidgetItem;
	item0->setText("0");
	item1->setText("0");
	table->setItem(0, 0, item0);
	table->setItem(0, 1, item1
	*/


}

void MainWindow::showType()
{
	drawGraph->setType(styleComboBox->itemData(styleComboBox->currentIndex(), Qt::UserRole).toInt());
}

void MainWindow::showColor()
{
	QColor color = QColorDialog::getColor(static_cast<int>(Qt::black), this);
	if (color.isValid())
	{
		//drawWidget->setColor(color);
		QPixmap p(20, 20);
		p.fill(color);
		colorBtn->setIcon(QIcon(p));
	}
}

void MainWindow::drawLineActionTrigger()
{
}

void MainWindow::runActionTrigger()
{

}

void MainWindow::drawEclipseActionTrigger()
{

}

void MainWindow::drawRectangleActionTrigger()
{

}

void MainWindow::drawTriangleActionTrigger()
{

}


void MainWindow::openFile()
{
	openFileAddr = QFileDialog::getOpenFileName(this, "Open File", "/", "png files(*.png *.jpg)");
	//drawWidget->openFile(openFileAddr);
}

bool MainWindow::saveAs()
{
	saveFileAddr = QFileDialog::getSaveFileName(this, "Save File", "/", "png files(*.png *.jpg)");
	if (saveFileAddr.isEmpty())
		return false;
	//drawWidget->saveFile(saveFileAddr);
	return true;
}

bool MainWindow::saveData()
{
	/*
	bool status = drawWidget->getSaveStatus();
	if (status == true) {
		return drawWidget->saveFile(saveFileAddr);
	}
	else {
		return saveAs();
	}
	*/
	return true;
}

bool MainWindow::saveGraph()
{
	return true;
}

void MainWindow::closeEvent(QCloseEvent * e)
{
	bool status = true;
	if (status == false)
	{
		QMessageBox box;
		box.setWindowTitle(tr("Warning"));
		box.setIcon(QMessageBox::Warning);
		box.setText(tr(" Unsaved, do you wnat to save?"));
		QPushButton* yesBtn = box.addButton(tr("Yes(&Y)"), QMessageBox::YesRole);
		box.addButton(tr("No(&N)"), QMessageBox::NoRole);
		QPushButton* cancelBut = box.addButton(tr("Cancel"), QMessageBox::RejectRole);
		box.exec();
		if (box.clickedButton() == yesBtn)
		{
			if (saveAs() == false)
				e->ignore();
			return;
		}
		else if (box.clickedButton() == cancelBut)
			e->ignore();
	}
}

void MainWindow::undoTrigger()
{

}

void MainWindow::redoTrigger()
{

}


/*
void MainWindow::setActionStatus()
{
	if(drawWidget->getUndoSize() > 0){
		undoAct->setEnabled(true);
	}else{
		undoAct->setEnabled(false);
	}
	if(drawWidget->getRedoSize() > 0){
		redoAct->setEnabled(true);
	}else{
		redoAct->setEnabled(false);
	}
}
*/

