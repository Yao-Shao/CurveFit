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
#include <QtDebug>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_updateSink(std::make_shared<updateSink>(this)),
	m_runSink(std::make_shared<runSink>(this)),
	fitType(LINEAR_FUNCTION)
{
	QWidget* centralWidget = new QWidget;
	setCentralWidget(centralWidget);
	setMinimumSize(LENGTH, WIDTH);

	setWindowTitle("OPCF");
	setWindowIcon(QIcon(":/OPCF/img/logo.png"));

	createMenu();
	createToolBar();
	createTable();
	createFuncText();
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
	QToolButton* runAction = new QToolButton();
	runAction->setIcon(QIcon(":/OPCF/img/running.png"));
	runAction->setToolTip(tr("Draw fitted curve"));
	toolBar->addWidget(runAction);
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

	connect(styleComboBox, SIGNAL(activated(int)), this, SLOT(showType()));
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
	table = new QTableWidget(ROW, COLUMN, this);
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

void MainWindow::createFuncText()
{
#ifndef DEBUG
	qDebug() << "In create Function Text\n";
#endif // !DEBUG


	functionText = new QPlainTextEdit(this);
	functionText->setGeometry(410, 600, 670, 100);
	QFont font = functionText->font(); 
	font.setPointSize(10);
	functionText->setFont(font);
	functionText->setPlainText("Hello world\nWellcome to our program...\n");
}

void MainWindow::createFuncView()
{
#ifndef NDEBUG
	qDebug() << "In create Function View\n";
#endif // !NDEBUG
	function_view = new QChart();
	function_view->setTitle("Function Curve");
	QLineSeries* series = new QLineSeries();
	qreal x, y;
	for(auto i = 0; i < real_xy_points->size(); i++) {
		x = ((*real_xy_points)[i]).getx();
		y = ((*real_xy_points)[i]).gety();
		series->append(x, y);
	}
	function_view->addSeries(series);
#ifndef NDEBUG
	qDebug() << " real_xy_points->size():\n"<<real_xy_points->size();
	qDebug() << "x range" << range_x->getx() << " to  " << range_x->gety() << "\n";
	qDebug()<<"y range "<< range_y->getx() << " to  " << range_y->gety() << "\n";
#endif // !NDEBUG
	QValueAxis* axisX = new QValueAxis;
	axisX->setRange(range_x->getx(), range_x->gety());
	axisX->setTitleText("x");
	axisX->setLabelFormat("%.3f");
	axisX->setTickCount(20);
	axisX->setMinorTickCount(4);

	QValueAxis* axisY = new QValueAxis;
	axisY->setRange(range_y->getx(), range_y->gety());
	axisY->setTitleText("y");
	axisY->setLabelFormat("%.3f"); 
	axisY->setTickCount(10);
	axisY->setMinorTickCount(4);
	function_view->setAxisX(axisX, series);
	function_view->setAxisY(axisY, series);
	chartview = new QChartView(function_view);
	chartview->setGeometry(410, 100, 670, 500);
	chartview->show();

}


void MainWindow::error_info()
{

	functionText->setPlainText("Sorry,we can't get a function from you sample points, check whether it's correct");
	functionText->show();
}

void MainWindow::run_error(const std::string& str)
{
	//show diffrent error infomation according to str
}

void MainWindow::showType()
{
	fitType = static_cast<Type>(styleComboBox->itemData(styleComboBox->currentIndex(), Qt::UserRole).toInt());
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


void MainWindow::getPoints()
{
#ifndef NDEBUG
	qDebug() << "In getPoints" << endl;
	qDebug() << "ROW: " << ROW;
#endif // !NDEBUG

	pointsData.clear();

	for (int i = 0; i < ROW; i++)
	{
		QTableWidgetItem* item1 = table->item(i, 0);
		QTableWidgetItem* item2 = table->item(i, 1); 
		if (item1 == NULL || item2 == NULL) {
			continue;
		}
		else {
			bool valid1 = true, valid2 = true;
			double data1 = (item1->text()).toDouble(&valid1);
			double data2 = (item2->text()).toDouble(&valid2);
			if (valid1 && valid2) {
				pointsData.push_back(Point(data1, data2));
				qDebug() << (item1->text()).toDouble() << item2->text().toDouble() << "\n";
			}
			else {
				continue;
			}
		}
	}

#ifndef NDEBUG
	qDebug() << "Points info " << endl;
	qDebug() << pointsData.size() << endl;
#endif // !NDEBUG

}

void MainWindow::set_function(std::shared_ptr<Function> spFunction)
{
	this->spFunction = spFunction;
}

void MainWindow::set_real_points(std::shared_ptr<Points> spRealPoints)
{
	this->real_xy_points = spRealPoints;
}

void MainWindow::set_range_x(std::shared_ptr<Point> range_xx)
{
	this->range_x = range_xx;
}

void MainWindow::set_range_y(std::shared_ptr<Point> range_yy)
{
	this->range_y = range_yy;
}

void MainWindow::update()
{

#ifndef NDEBUG
	qDebug() << "update" << QString::fromStdString(spFunction->get_function()) << endl;
#endif // !NDEBUG
	functionText->setPlainText("Run successfully, and the function is: \n y = " + QString::fromStdString(spFunction->get_function()));
	functionText->show();
	createFuncView();

}

void MainWindow::runActionTrigger()
{
#ifndef NDEBUG
	qDebug() << "In runAction Trigger" << endl;
#endif // !NDEBUG

	getPoints();

#ifndef NDEBUG
	qDebug() << "Out of getPoints" << endl;
	qDebug() << pointsData.size() << endl;
#endif // !NDEBUG

	m_param.set_type(fitType);
	m_param.set_point(pointsData);
	m_cmdRun->SetParameter(m_param);
	m_cmdRun->Exec();

#ifndef NDEBUG
	qDebug() << "End of pass para" << endl;
#endif // !NDEBUG
}

std::shared_ptr<IPropertyNotification> MainWindow::get_updateSink()
{
	return std::static_pointer_cast<IPropertyNotification>(m_updateSink);
}

std::shared_ptr<ICommandNotification> MainWindow::get_runSink()
{
	return std::static_pointer_cast<ICommandNotification>(m_runSink);
}

void MainWindow::set_runCommand(const std::shared_ptr<ICommandBase>& cmd)
{
	m_cmdRun = cmd;
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










void MainWindow::drawLineActionTrigger()
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

void MainWindow::closeEvent(QCloseEvent* e)
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

