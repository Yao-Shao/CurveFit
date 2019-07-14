#include <iostream>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QTableWidget>
#include <QColorDialog>
#include <QFileDialog>
#include <QFileDevice>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QMessageBox>
#include <QAbstractButton>
#include <QPushButton>
#include <QtDebug>
#include <QGridLayout>
#include <algorithm>
#include <ActiveQt\QAxObject>
#include "mainwindow.h"
#include "math.h"

#define NDEBUG
#define DIFFBOUND 1e-6


MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	m_updateSink(std::make_shared<updateSink>(this)),
	m_runSink(std::make_shared<runSink>(this)),
	chartView(new QChartView(this)),
	m_layout(new QGridLayout(this)),
	fitType(LINEAR_FUNCTION)
{
	centralWidget = new QWidget;
	setCentralWidget(centralWidget);

	//centralWidget->setMouseTracking(true);

	setMinimumSize(LENGTH, WIDTH);
	showMaximized();

	setWindowTitle("OPCF");
	setWindowIcon(QIcon(":/OPCF/img/logo.png"));

	/* set background color */
	QPalette m_palette(centralWidget->palette());
	m_palette.setColor(QPalette::Background, QColor(49,54,59));
	centralWidget->setAutoFillBackground(true);
	centralWidget->setPalette(m_palette);

	myPix.load(":/OPCF/img/run_error.png");
	error_label_pic = new QLabel(this);
	m_valueLabel = new QLabel(this);
	m_valueLabel->setGeometry(100, 100, 130, 20);
	FileIsNew = true;
	undo_flag = false;
	redo_flag = false;

	initFuncView = true;
	pressAddingBtn = false;
	whether_move_point = false;


	basePoints = new QScatterSeries();
	axisX = new QValueAxis(this);
	axisY = new QValueAxis(this);
	function_view = new QChart();
	all_points = new QScatterSeries(this);
	series = new QLineSeries(this);
	samplepoints = new QScatterSeries(this);
	samplepoints_o = new QScatterSeries(this);

	function_view->addSeries(all_points);
	function_view->addSeries(samplepoints);
	function_view->addSeries(samplepoints_o);
	function_view->addSeries(basePoints);
	function_view->addSeries(series);

	function_view->setAxisX(axisX, basePoints);
	function_view->setAxisY(axisY, basePoints);

	function_view->setAxisX(axisX, series);
	function_view->setAxisY(axisY, series);

	function_view->setAxisX(axisX, all_points);
	function_view->setAxisY(axisY, all_points);

	function_view->setAxisX(axisX, samplepoints);
	function_view->setAxisY(axisY, samplepoints);

	function_view->setAxisX(axisX, samplepoints_o);
	function_view->setAxisY(axisY, samplepoints_o);


	series->setBrush(QColor(102, 151, 210));
	series->setColor(QColor(102, 151, 210));

	all_points->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	all_points->setBorderColor(QColor(102, 151, 210));
	all_points->setBrush(QColor(102, 151, 210));
	all_points->setMarkerSize(1);

	samplepoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	samplepoints->setBorderColor(QColor(102, 151, 210));
	samplepoints->setBrush(QColor(102, 151, 210));
	samplepoints->setMarkerSize(20);

	samplepoints_o->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	samplepoints_o->setBorderColor(Qt::white);
	samplepoints_o->setBrush(Qt::white);
	samplepoints_o->setMarkerSize(10);

	basePoints->setMarkerShape(QScatterSeries::MarkerShapeCircle);
	basePoints->setBorderColor(Qt::white);
	basePoints->setBrush(Qt::white);
	basePoints->setMarkerSize(5);

	createMenu();
	createToolBar();
	createTable();
	createFuncText();
	InitFuncView();
	setLayout();

}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenu()
{
	/* File */
	QMenuBar* pmenuBar = menuBar();
	QMenu* fileMenu = pmenuBar->addMenu("File");


	QAction* openAction = new QAction("Open Project");
	openAction->setShortcut((Qt::CTRL | Qt::Key_O));
	fileMenu->addAction(openAction);
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

	QAction * loadExcelAction = new QAction("Load Excel");
	loadExcelAction->setShortcut((Qt::CTRL | Qt::Key_E));
	fileMenu->addAction(loadExcelAction);
	connect(loadExcelAction, SIGNAL(triggered()), this, SLOT(loadExcelFile()));

	QAction * saveDataAction = new QAction("Save Project");
	saveDataAction->setShortcut((Qt::CTRL | Qt::Key_S));
	fileMenu->addAction(saveDataAction);
	connect(saveDataAction, SIGNAL(triggered()), this, SLOT(saveData()));

	QAction * saveGraphAction = new QAction("Save graph", this);
	saveGraphAction->setShortcut((Qt::CTRL | Qt::ALT | Qt::Key_S));
	fileMenu->addAction(saveGraphAction);
	connect(saveGraphAction, SIGNAL(triggered()), this, SLOT(saveGraph()));

	QAction * saveAsAction = new QAction("Save as", this);
	saveAsAction->setShortcut((Qt::CTRL | Qt::SHIFT | Qt::Key_S));
	fileMenu->addAction(saveAsAction);
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

	/* Edit */
	QMenu * editMenu = pmenuBar->addMenu("Edit");

	QAction * redoAction = new QAction("Redo", this);
	redoAction->setShortcut((Qt::CTRL | Qt::Key_R));
	editMenu->addAction(redoAction);
	connect(redoAction, SIGNAL(triggered()), this, SLOT(redoTrigger()));

	QAction * undoAction = new QAction("Undo", this);
	undoAction->setShortcut((Qt::CTRL | Qt::Key_Z));
	editMenu->addAction(undoAction);
	connect(undoAction, SIGNAL(triggered()), this, SLOT(undoTrigger()));

	QMenu * helpMenu = pmenuBar->addMenu("Help");
	QAction * seekHelp = new QAction("View Help");
	seekHelp->setShortcut((Qt::CTRL | Qt::Key_H));
	helpMenu->addAction(seekHelp);
	connect(seekHelp, SIGNAL(triggered()), this, SLOT(openHelpFile()));

}

void MainWindow::createToolBar()
{
	QToolBar* toolBar = addToolBar("Tool");             /* add tool bar object */

	/* Runing */
	QToolButton* runAction = new QToolButton(this);
	runAction->setShortcut(Qt::CTRL | Qt::Key_R);
	runAction->setIcon(QIcon(":/OPCF/img/running.png"));
	runAction->setToolTip(tr("Show fit curve"));
	toolBar->addWidget(runAction);
	connect(runAction, SIGNAL(clicked()), this, SLOT(runActionTrigger()));


	/* fit type */
	fitTypeComboBox = new QComboBox(this);
	fitTypeComboBox->addItem(tr("Line"), static_cast<int>(LINEAR_FUNCTION));
	fitTypeComboBox->addItem(tr("Quad"), static_cast<int>(QUADRATIC_FUNCTION));
	fitTypeComboBox->addItem(tr("Cubic(x^3)"), static_cast<int>(CUBIC_FUNCTION));
	fitTypeComboBox->addItem(tr("Log"), static_cast<int>(LN_FUNCTION));
	fitTypeComboBox->addItem(tr("Exponential"), static_cast<int>(EXPONENTIAL_FUNCTION));
	fitTypeComboBox->addItem(tr("CubicSpline"), static_cast<int>(NORMAL_FUNCTION));
	connect(fitTypeComboBox, SIGNAL(activated(int)), this, SLOT(showType()));
	toolBar->addWidget(fitTypeComboBox);

	toolBar->addSeparator();

	/* adding points */
	QAction* isAddingAction = new QAction("Add Point", toolBar);
	isAddingAction->setIcon(QIcon(":/OPCF/img/dot.png"));
	isAddingAction->setToolTip(tr("Add Point"));
	isAddingAction->setCheckable(true);
	toolBar->addAction(isAddingAction);
	connect(isAddingAction, SIGNAL(changed()), this, SLOT(isAddingActionTrigger()));

	/*derived function*/
	QToolButton * showDerivedAction = new QToolButton(this);
	showDerivedAction->setIcon(QIcon(":/OPCF/img/showD.png"));
	showDerivedAction->setToolTip(tr("Show Derived function"));
	toolBar->addWidget(showDerivedAction);
	connect(showDerivedAction, SIGNAL(clicked()), this, SLOT(showDerivedActionTrigger()));

	toolBar->addSeparator();

	/* save */
	QToolButton * saveAction = new QToolButton(this);
	saveAction->setIcon(QIcon(":/OPCF/img/save.png"));
	saveAction->setToolTip(tr("Save project file"));
	toolBar->addWidget(saveAction);
	connect(saveAction, SIGNAL(clicked()), this, SLOT(saveData()));

	/* undo */
	QToolButton * undoAction = new QToolButton(this);
	undoAction->setIcon(QIcon(":/OPCF/img/undo.png"));
	undoAction->setToolTip(tr("Undo"));
	toolBar->addWidget(undoAction);
	connect(undoAction, SIGNAL(clicked()), this, SLOT(undoTrigger()));

	/* redo */
	QToolButton * redoAction = new QToolButton(this);
	redoAction->setIcon(QIcon(":/OPCF/img/redo.png"));
	redoAction->setToolTip(tr("Redo"));
	toolBar->addWidget(redoAction);
	connect(redoAction, SIGNAL(clicked()), this, SLOT(redoTrigger()));
}

void MainWindow::createTable()
{
	table = new QTableWidget(ROW, COLUMN, this);

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
}

void MainWindow::createFuncText()
{
#ifndef NDEBUG
	qDebug() << "In create Function Text\n";
#endif // !NDEBUG

	functionText = new QPlainTextEdit(this);
	functionText->setReadOnly(true);

	QFont font = functionText->font();
	font.setPointSize(10);
	functionText->setFont(font);

	functionText->setPlainText("Hello world\nWellcome to our program...\n");
	functionText->show();
}

void MainWindow::createFuncView()
{
#ifndef NDEBUG
	qDebug() << "In create Function View\n";
#endif // !NDEBUG
	all_points->clear();
	series->clear();
	qreal x, y;
	for (auto i = 0; i < real_xy_points->size(); i++) {
		x = ((*real_xy_points)[i]).getx();
		y = ((*real_xy_points)[i]).gety();
		series->append(x, y);
		all_points->append(x, y);
	}
	samplepoints->clear();
	samplepoints_o->clear();
	for (auto i = 0; i < sample_points->size(); i++) {
		x = ((*sample_points)[i]).getx();
		y = ((*sample_points)[i]).gety();
		samplepoints->append(x, y);
		samplepoints_o->append(x, y);
	}




#ifndef NDEBUG
	qDebug() << " real_xy_points->size():\n" << real_xy_points->size();
	qDebug() << "x range" << range_x->getx() << " to  " << range_x->gety() << "\n";
	qDebug() << "y range " << range_y->getx() << " to  " << range_y->gety() << "\n";
#endif // !NDEBUG
	double start_x = range_x->getx();
	double end_x = range_x->gety();
	double start_y = range_y->getx();
	double end_y = range_y->gety();
	if (start_y == end_y) {
		double length_x = end_x - start_x;

		start_x = floor((start_x - length_x / 10) * 100) / 100;
		end_x = ceil((end_x + length_x / 10) * 100) / 100;
		start_y = start_y - 1;
		end_y = end_y + 1;
	}
	else {
		double length_x = end_x - start_x;
		double length_y = end_y - start_y;

		start_x = floor((start_x - length_x / 10) * 100) / 100;
		end_x = ceil((end_x + length_x / 10) * 100) / 100;
		start_y = floor((start_y - length_y / 10) * 100) / 100;
		end_y = ceil((end_y + length_y / 10) * 100) / 100;
	}


	axisX->setRange(start_x, end_x);
	axisX->setTitleText("x");
	axisX->setLabelFormat("%.2f");
	axisX->setTickCount(21);
	axisX->setMinorTickCount(4);

	axisY->setRange(start_y, end_y);
	axisY->setTitleText("y");
	axisY->setLabelFormat("%.2f");
	axisY->setTickCount(11);
	axisY->setMinorTickCount(4);

	/*show points' value*/
	connect(samplepoints_o, &QScatterSeries::hovered, this, &MainWindow::slotPointHoverd);
	connect(all_points, &QScatterSeries::hovered, this, &MainWindow::slotPointHoverd);
	connect(samplepoints_o, &QScatterSeries::doubleClicked, this, &MainWindow::movePoint);

	/*
	basePoints->append(start_x, end_y);
	basePoints->append(end_x,start_y);
	function_view->addSeries(basePoints);
	function_view->setAxisX(axisX, basePoints);
	function_view->setAxisY(axisY, basePoints);
	*/
	//chartView->setMouseTracking(true);
	chartView->setChart(function_view);
	chartView->show();
}

void MainWindow::InitFuncView()
{
	initFuncView = false;

	axisX->setRange(-100, 100);
	axisX->setTitleText("x");
	axisX->setLabelFormat("%.1f");
	axisX->setTickCount(21);
	axisX->setMinorTickCount(4);


	axisY->setRange(-100, 100);
	axisY->setTitleText("y");
	axisY->setLabelFormat("%.1f");
	axisY->setTickCount(11);
	axisY->setMinorTickCount(4);

	basePoints->append(-100, 100);
	basePoints->append(100, -100);


	function_view->setTheme(QChart::ChartThemeBrownSand);
	chartView->setChart(function_view);
	chartView->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent * e)
{

	if (!initFuncView) {
		// Setting the mouse position label on the axis from value to position
		auto const widgetPos = e->pos();
		auto const scenePos = chartView->mapFromGlobal(widgetPos);
		auto const chartItemPos = chartView->mapToScene(scenePos);
		auto const picval = chartView->chart()->mapFromScene(chartItemPos);
		QPointF pos;
		pos.setX(picval.x());
		pos.setY(picval.y() + 30);
		auto const pickVal = chartView->chart()->mapToValue(pos, function_view->series().at(0));


		m_valueLabel->setText(QString::asprintf("%.3f,%.3f", pickVal.x(), pickVal.y()));

		QPoint curPos = e->pos();
		m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);

		m_valueLabel->show();
	}
	//mouseMoveEvent(e);

}

void MainWindow::mousePressEvent(QMouseEvent * e)
{

	qDebug() << "pres" << endl;
	if (!initFuncView) {
		auto inScene = function_view->plotArea();
		auto inChart = function_view->mapFromScene(inScene);
		auto inChartRect = inChart.boundingRect();
		auto inItem1 = chartView->chart()->mapToValue(inChartRect.topLeft(), function_view->series().at(0));
		auto inItem2 = chartView->chart()->mapToValue(inChartRect.bottomRight(), function_view->series().at(0));
		auto const widgetPos = e->pos();
		auto const scenePos = chartView->mapFromGlobal(widgetPos);
		auto const chartItemPos = chartView->mapToScene(scenePos);
		auto const picval = chartView->chart()->mapFromScene(chartItemPos);
		QPointF pos;
		pos.setX(picval.x());
		pos.setY(picval.y() + 30);
		auto const pickVal = chartView->chart()->mapToValue(pos, function_view->series().at(0));
#ifndef NDEBUG
		qDebug() << " in sence " << inScene;
		qDebug() << " in Chart " << inChart;
		qDebug() << " in Chart Rect " << inChartRect;
		qDebug() << "inItems " << inChartRect.topLeft();
		qDebug() << " inItem2" << pos;
		qDebug() << "widgetPos:" << widgetPos;
		qDebug() << "scenePos:" << scenePos;
		qDebug() << "chartItemPos:" << chartItemPos;
		qDebug() << "valSeries:" << picval;
#endif // !NDEBUG

		if (pressAddingBtn) {
			samplepoints->append(pickVal);
			samplepoints_o->append(pickVal);
			chartView->update();
			addPoint(pickVal);
		}
		if (whether_move_point) {
			QTableWidgetItem* item1 = new QTableWidgetItem;
			QTableWidgetItem* item2 = new QTableWidgetItem;
			item1->setText(QString::number(pickVal.x()));
			item2->setText(QString::number(pickVal.x()));
			table->setItem(movepoint_row, 0, item1);
			table->setItem(movepoint_row, 1, item2);
			qDebug() << (item1->text()).toDouble() << item2->text().toDouble() << "\n";
			table->update();
			runActionTrigger();
			whether_move_point = false;
		}
	}
}

void MainWindow::movePoint(const QPointF & p)
{
	qDebug() << "Qpoints" << endl;
	whether_move_point = true;
	//pointsData.push_back(Point(p.rx(), p.ry()));
	for (int i = 0; i < ROW; i++)
	{

		double data1 = table->item(i, 0)->text().toDouble();
		double data2 = table->item(i, 1)->text().toDouble();
		if ((abs((data1 - p.x())) < DIFFBOUND) && (abs((data2 - p.y()) < DIFFBOUND))) {
			movepoint_row = i;
			break;
		}
	}
	qDebug() << "Row " << movepoint_row << endl;
	return;

}

void MainWindow::mouseReleaseEvent(QMouseEvent * e)
{
	qDebug() << "hello" << endl;
	m_valueLabel->hide();
	//setMouseTracking(true);

}

bool MainWindow::addPoint(QPointF p)
{
	//pointsData.push_back(Point(p.rx(), p.ry()));
	for (int i = 0; i < ROW; i++)
	{
		QTableWidgetItem* item1 = table->item(i, 0);
		QTableWidgetItem* item2 = table->item(i, 1);
		if (item1 == NULL || (item1->text() == "") || item2 == NULL || (item2->text() == "")) {
			if (item1 != NULL)
				delete item1;
			if (item2 != NULL)
				delete item2;
			item1 = new QTableWidgetItem;
			item2 = new QTableWidgetItem;
			item1->setText(QString::number(p.rx()));
			item2->setText(QString::number(p.ry()));
			table->setItem(i, 0, item1);
			table->setItem(i, 1, item2);
			break;
		}
	}
	return true;
}

void MainWindow::error_info()
{
	functionText->setPlainText("Sorry,we can't get a function from you sample points, check whether it's correct");
}

void MainWindow::run_error(const std::string & str)
{
	chartView->close();
	//show diffrent error infomation according to str
	if (str == "NoSamplePoints") {
		functionText->setPlainText("Run error C0001:  There is no sample points in the table...");
	}
	else if (str == "NotEnoughForCubic") {
		functionText->setPlainText("Run error C0002:  Not enough sample points for Cubic Spline Method...");
	}
	else if (str == "conflictPoints") {
		functionText->setPlainText("Run error C0003: Multiple points with the same x but different y");
	}
	else if (str == "Lnwithwrongx") {
		functionText->setPlainText("Run error C0004: The x points int the ln fit should be positive");
	}
	else if (str == "Expwithwrongy") {
		functionText->setPlainText("Run error C0005: The y points int the Exp fit should be positive");
	}
	else if (str == "Can'thavesolution") {
		functionText->setPlainText("Run error C0006: There is some thing wrong with your sample points, can't fit cubic function from it");
	}
	error_label_pic->setPixmap(myPix);
	error_label_pic->show();
}

void MainWindow::showType()
{
	fitType = static_cast<Type>(fitTypeComboBox->itemData(fitTypeComboBox->currentIndex(), Qt::UserRole).toInt());
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


bool MainWindow::getPoints()
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

	return checkPoints();
}

bool MainWindow::checkPoints()
{
	if (pointsData.size() == 0)
		return true;
	bool flag = true;
	std::sort(pointsData.begin(), pointsData.end());
	for (int i = 0; i < pointsData.size() - 1; i++) {
		if (pointsData[i].getx() == pointsData[i + 1].getx()) {
			if (pointsData[i].gety() != pointsData[i + 1].gety()) {
				return false;
			}
			else {
				pointsData.erase(pointsData.begin() + i);
			}
		}
	}
	return true;
}

void MainWindow::set_function(std::shared_ptr<Function> spFunction)
{
	this->spFunction = spFunction;
}

void MainWindow::set_real_points(std::shared_ptr<Points> spRealPoints)
{
	this->real_xy_points = spRealPoints;
}

void MainWindow::set_sample_points(std::shared_ptr<Points> spsamplePoints)
{
	this->sample_points = spsamplePoints;
}

void MainWindow::set_dy_points(std::shared_ptr<Points> dyPoints)
{
	this->dyPoints = dyPoints;
}

void MainWindow::set_range_x(std::shared_ptr<Point> range_xx)
{
	this->range_x = range_xx;
}

void MainWindow::set_range_y(std::shared_ptr<Point> range_yy)
{
	this->range_y = range_yy;
}

void MainWindow::runActionTrigger()
{
#ifndef NDEBUG
	qDebug() << "In runAction Trigger" << endl;
#endif // !NDEBUG

	bool rep = getPoints();
	if (rep == false)
	{
		run_error("conflictPoints");
		return;
	}

#ifndef NDEBUG
	qDebug() << "Out of getPoints" << endl;
	qDebug() << pointsData.size() << endl;
#endif // !NDEBUG

	m_param.set_type(fitType);
	m_param.set_point(pointsData);
	m_cmdRun->SetParameter(m_param);
	m_cmdRun->Exec();
	FileChanged = true;

	Param_opcf t = m_param;
	undo_stack.push(t);
#ifndef NDEBUG
	qDebug() << "End of pass para" << endl;
#endif // !NDEBUG
}

void MainWindow::isAddingActionTrigger()
{
	pressAddingBtn = !pressAddingBtn;
}

void MainWindow::update()
{
	error_label_pic->close();
#ifndef NDEBUG
	qDebug() << "update" << QString::fromStdString(spFunction->get_function()) << endl;
#endif // !NDEBUG

	functionText->setPlainText("Run successfully, and the function is: \n y = " + QString::fromStdString(spFunction->get_function()));
	createFuncView();
}

std::shared_ptr<IPropertyNotification> MainWindow::get_updateSink()
{
	return std::static_pointer_cast<IPropertyNotification>(m_updateSink);
}

std::shared_ptr<ICommandNotification> MainWindow::get_runSink()
{
	return std::static_pointer_cast<ICommandNotification>(m_runSink);
}

void MainWindow::set_runCommand(const std::shared_ptr<ICommandBase> & cmd)
{
	m_cmdRun = cmd;
}

void MainWindow::setLayout()
{

	//chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//table->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	//functionText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	table->setMaximumWidth(400);
	error_label_pic->setAlignment(Qt::AlignCenter);
	m_layout->addWidget(table, 0, 0, 2, 1);
	m_layout->addWidget(chartView, 0, 1);
	m_layout->addWidget(error_label_pic, 0, 1);
	m_layout->addWidget(functionText, 1, 1);

	m_layout->setColumnStretch(0, 3);
	m_layout->setColumnStretch(1, 5);
	m_layout->setRowStretch(0, 3);
	m_layout->setRowStretch(1, 1);

	centralWidget->setLayout(m_layout);

#ifndef NDEBUG
	qDebug() << m_layout->rowCount() << " " << m_layout->columnCount() << "/n";
#endif
}

void MainWindow::showDerivedActionTrigger()
{
	if (dyPoints->size() <= 0) {
		bool intable = false;
		for (int i = 0; i < ROW; i++)
		{
			QTableWidgetItem* item1 = table->item(i, 0);
			QTableWidgetItem* item2 = table->item(i, 1);
			if (item1 == NULL || item2 == NULL) {
				continue;
			}
			else {
				intable = true;
				break;
			}
		}
		if (intable) {
			QMessageBox::warning(this, "error", "You shou fit the function before get it's derived function");
		}
		else {
			QMessageBox::warning(this, "error", "No sample points");
		}
	}
	else {
		QChartView* DyChartView = new QChartView();
		QChart* dy_function_view = new QChart();
		dy_function_view->setTheme(QChart::ChartThemeBrownSand);
#ifndef NDEBUG
		qDebug() << "showDerivedActionTrigger()\n";
#endif // !NDEBUG
		//function_view->setTitle("Function Curve");
		QLineSeries* series = new QLineSeries(this);
		qreal x, y;
		for (auto i = 0; i < dyPoints->size(); i++) {
			x = ((*dyPoints)[i]).getx();
			y = ((*dyPoints)[i]).gety();
			series->append(x, y);
		}
		dy_function_view->addSeries(series);
#ifndef NDEBUG
		qDebug() << " dy_points->size():\n" << dyPoints->size() << "\n";
		qDebug() << "x range" << range_x->getx() << " to  " << range_x->gety() << "\n";
		qDebug() << "y range " << range_y->getx() << " to  " << range_y->gety() << "\n";
#endif // !NDEBUG
		double start_x = range_x->getx();
		double end_x = range_x->gety();
		double start_y = ((*dyPoints)[0]).gety();
		for (auto i = 1; i < dyPoints->size(); i++) {
			if (start_y > ((*dyPoints)[i]).gety())
			{
				start_y = ((*dyPoints)[i]).gety();
			}
		}
		double end_y = ((*dyPoints)[0]).gety();
		for (auto i = 1; i < dyPoints->size(); i++) {
			if (end_y < ((*dyPoints)[i]).gety())
			{
				end_y = ((*dyPoints)[i]).gety();
			}
		}
		if (start_y == end_y) {
			double length_x = end_x - start_x;

			start_x = floor((start_x - length_x / 10) * 100) / 100;
			end_x = ceil((end_x + length_x / 10) * 100) / 100;
			start_y = start_y - 1;
			end_y = end_y + 1;
		}
		else {
			double length_x = end_x - start_x;
			double length_y = end_y - start_y;

			start_x = floor((start_x - length_x / 10) * 100) / 100;
			end_x = ceil((end_x + length_x / 10) * 100) / 100;
			start_y = floor((start_y - length_y / 10) * 100) / 100;
			end_y = ceil((end_y + length_y / 10) * 100) / 100;
		}

		QValueAxis* axisX = new QValueAxis(this);
		axisX->setRange(start_x, end_x);
		axisX->setTitleText("x");
		axisX->setLabelFormat("%.2f");
		axisX->setTickCount(21);
		axisX->setMinorTickCount(4);

		QValueAxis* axisY = new QValueAxis(this);
		axisY->setRange(start_y, end_y);
		axisY->setTitleText("y");
		axisY->setLabelFormat("%.2f");
		axisY->setTickCount(11);
		axisY->setMinorTickCount(4);

		dy_function_view->setAxisX(axisX, series);
		dy_function_view->setAxisY(axisY, series);

		DyChartView->setChart(dy_function_view);
		DyChartView->setGeometry(400, 400, 600, 500);
		DyChartView->show();
	}

}


void MainWindow::openFile() {

	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text File(*.txt)");
	if (fileName == "")
	{
		return;
	}
	else
	{
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, "error", "open file error!");
			return;
		}
		else
		{
			if (!file.isReadable())
				QMessageBox::warning(this, "error", "this file is not readable!");
			else
			{
				if (FileChanged == true)
				{
					QMessageBox box;
					box.setWindowTitle(tr("Warning"));
					box.setIcon(QMessageBox::Warning);
					box.setText(tr(" Unsaved, do you want to save?"));
					QPushButton* yesBtn = box.addButton(tr("Yes(&Y)"), QMessageBox::YesRole);
					box.addButton(tr("No(&N)"), QMessageBox::NoRole);
					box.exec();
					if (box.clickedButton() == yesBtn)
					{
						if (FileIsNew) {
							this->saveAs();
						}
						else
						{
							this->saveData();
						}
					}
				}
				//将文件数据导入表格
				int r_count = 0;        //统计文件的行数
				QStringList textList;   //记录文件中每一行的数据
				QTextStream in(&file);
				QString t = in.readLine();
				while (!in.atEnd())
				{
					QString line = in.readLine();
					textList.append(line);          //保存文件的数据
					r_count++;                      //记录文件的行数 前两行为表头
				}
				file.close();
				table->clear();
				if (!textList.isEmpty()) {
					for (int i = 0; i < r_count; i++) {
						QStringList temp;
						temp = textList.at(i).split("\t");
						for (int j = 0; j < 2; j++) {
							QTableWidgetItem* item = new QTableWidgetItem(temp.at(j));
							table->setItem(i, j, item);
						}
					}
				}
				table->show();
				flag_isOpen = 1;
				LastFileName = fileName;
				fitType = (Type)t.toInt();
				m_param.set_type(fitType);
				m_param.set_point(pointsData);
				m_cmdRun->SetParameter(m_param);
				m_cmdRun->Exec();
				FileChanged = false;
			}
		}
	}
}


bool MainWindow::saveAs()
{
	if (sample_points->size() == 0)
	{
		QMessageBox::warning(this, "error", "content can not be none!", QMessageBox::Ok);
	}
	else
	{
		QFileDialog fileDialog;
		QString str = fileDialog.getSaveFileName(this, "Open File", "", "Text File(*.txt)");
		if (str == "")
		{
			return false;
		}
		QFile filename(str);
		if (!filename.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, "error", "Open File Error!");
			return false;
		}
		else
		{
			QTextStream textStream(&filename);
			Type t = spFunction->get_type();
			textStream << t << "\n";
			for (auto i = 0; i < sample_points->size(); i++) {
				double x, y;
				x = (*sample_points)[i].getx();
				y = (*sample_points)[i].gety();
				textStream << x << "\t" << y << "\n";
			}

		}
		FileChanged = false;
		QMessageBox::information(this, "Save File", "Save File Success", QMessageBox::Ok);
		filename.close();
		FileIsNew = false;
		LastFileName = str;
	}
	return true;
}

bool MainWindow::saveData()
{
	if (FileIsNew)
	{
		if (sample_points->size() == 0)
		{
			QMessageBox::warning(this, "error", "content can not be none!", QMessageBox::Ok);
		}
		else
		{
			QFileDialog fileDialog;
			QString str = fileDialog.getSaveFileName(this, "Open File", "", "Text File(*.txt)");
			if (str == "")
			{
				return false;
			}
			QFile filename(str);
			if (!filename.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QMessageBox::warning(this, "error", "Open File Error!");
				return false;
			}
			else
			{
				QTextStream textStream(&filename);
				Type t = spFunction->get_type();
				textStream << t << "\n";
				for (auto i = 0; i < sample_points->size(); i++) {
					double x, y;
					x = (*sample_points)[i].getx();
					y = (*sample_points)[i].gety();
					textStream << x << "\t" << y << "\n";
				}

			}
			FileChanged = false;
			QMessageBox::information(this, "Save File", "Save File Success", QMessageBox::Ok);
			filename.close();
			FileIsNew = false;
			LastFileName = str;
		}
	}
	else
	{
		if (flag_isOpen)
		{
			QFile file(LastFileName);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			{
				QMessageBox::warning(this, "error", "Open File Faile");
				return false;
			}
			else
			{
				QTextStream textStream(&file);
				Type t = spFunction->get_type();
				textStream << t << "\n";
				for (auto i = 0; i < sample_points->size(); i++) {
					double x, y;
					x = (*sample_points)[i].getx();
					y = (*sample_points)[i].gety();
					textStream << x << "\t" << y << "\n";
				}
				file.close();
				QMessageBox::information(this, "Save File", "Save File Success", QMessageBox::Ok);
				FileChanged = false;
			}
		}
		else
		{
			QMessageBox::warning(this, "Warning", "Please new or open a file");
			return true;
		}
	}
	return true;
}

bool MainWindow::saveGraph()
{
	QScreen* screen = QGuiApplication::primaryScreen();
	QPixmap p = screen->grabWindow(chartView->winId());
	QFileDialog fileDialog;
	QString str = fileDialog.getSaveFileName(this, "Save Pic", "", "Text File(*.png)");
	if (str == "")
	{
		return false;
	}
	else {
		QImage image = p.toImage();
		image.save(str);
		QMessageBox::information(this, "Save Pic", "Save Picture Success", QMessageBox::Ok);
	}
}

void MainWindow::closeEvent(QCloseEvent * e)
{

	if (FileChanged == true)
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
			if (FileIsNew) {
				this->saveAs();
			}
			else
			{
				this->saveData();
			}
			return;
		}
		else if (box.clickedButton() == cancelBut)
			e->ignore();
	}
}

void MainWindow::undoTrigger()
{
	if (undo_stack.size() > 1) {
		Param_opcf t = undo_stack.top();
		undo_stack.pop();
		redo_stack.push(t);
		t = undo_stack.top();
		table->clear();
		Points p = t.get_points();
		for (int i = 0; i < p.size(); i++) {
			double x = p[i].getx();
			double y = p[i].gety();
			QString s_x = QString::number(x);
			QString s_y = QString::number(y);
			QTableWidgetItem* item = new QTableWidgetItem(s_x);
			table->setItem(i, 0, item);
			QTableWidgetItem* item2 = new QTableWidgetItem(s_y);
			table->setItem(i, 1, item2);
		}
		table->show();
		m_param = t;
		m_cmdRun->SetParameter(m_param);
		m_cmdRun->Exec();
		FileChanged = true;
	}
	else {
		QMessageBox::information(this, "Undo", "No more steps", QMessageBox::Ok);
	}
}

void MainWindow::redoTrigger()
{
	if (redo_stack.size() > 0) {
		Param_opcf t = redo_stack.top();
		redo_stack.pop();
		undo_stack.push(t);
		table->clear();
		Points p = t.get_points();
		for (int i = 0; i < p.size(); i++) {
			double x = p[i].getx();
			double y = p[i].gety();
			QString s_x = QString::number(x);
			QString s_y = QString::number(y);
			QTableWidgetItem* item = new QTableWidgetItem(s_x);
			table->setItem(i, 0, item);
			QTableWidgetItem* item2 = new QTableWidgetItem(s_y);
			table->setItem(i, 1, item2);
		}
		m_param = t;
		m_cmdRun->SetParameter(m_param);
		m_cmdRun->Exec();
		FileChanged = true;
	}
	else {
		QMessageBox::information(this, "Rndo", "No more steps", QMessageBox::Ok);
	}
}

void MainWindow::slotPointHoverd(const QPointF & point, bool state)
{
	if (state) {
		m_valueLabel->setText(QString::asprintf("%.3f,%.3f", point.x(), point.y()));

		QPoint curPos = mapFromGlobal(QCursor::pos());
		m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);

		m_valueLabel->show();
	}
	else
		m_valueLabel->hide();

}


void MainWindow::openHelpFile()
{

	QString qtManulFile = "HelpDoc/HelpDoc.pdf";

	QDesktopServices::openUrl(QUrl::fromLocalFile(qtManulFile));
}

void MainWindow::loadExcelFile()
{
	functionText->setPlainText("Loading excel file, please wait for a second :)");
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text File(*.xlsx *.xls)");
	if (NULL == fileName || fileName == "")
	{
		functionText->setPlainText("Loading failed, no file seleted!");
		return;
	}
	else
	{
		QFile file(fileName);
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			QMessageBox::warning(this, "error", "open file error!");
			return;
		}
		else
		{
			if (!file.isReadable())
				QMessageBox::warning(this, "error", "this file is not readable!");
			else
			{
				if (FileChanged == true)
				{
					QMessageBox box;
					box.setWindowTitle(tr("Warning"));
					box.setIcon(QMessageBox::Warning);
					box.setText(tr(" Unsaved, do you want to save?"));
					QPushButton* yesBtn = box.addButton(tr("Yes(&Y)"), QMessageBox::YesRole);
					box.addButton(tr("No(&N)"), QMessageBox::NoRole);
					box.exec();
					if (box.clickedButton() == yesBtn)
					{
						if (FileIsNew) {
							this->saveAs();
						}
						else
						{
							this->saveData();
						}
					}
				}
				table->clear();
				QAxObject excel("Excel.Application");
				excel.setProperty("Visible", false);
				QAxObject* workbooks = excel.querySubObject("WorkBooks");
				QAxObject* workbook = workbooks->querySubObject("Open(QString, QVariant)", fileName);
				QAxObject* worksheet = workbook->querySubObject("WorkSheets(int)", 1);
				QAxObject* usedrange = worksheet->querySubObject("UsedRange");
				QAxObject* rows = usedrange->querySubObject("Rows");
				int intRows = rows->property("Count").toInt();

				QString Range = "A1:B" + QString::number(intRows);
				QAxObject* allEnvData = worksheet->querySubObject("Range(QString)", Range);
				QVariant allEnvDataQVariant = allEnvData->property("Value");
				QVariantList allEnvDataList = allEnvDataQVariant.toList();
				if (intRows > ROW) {
					intRows = ROW;
				}
				for (int i = 0; i < intRows; i++)
				{
					QVariantList allEnvDataList_i = allEnvDataList[i].toList();
					QString data1 = allEnvDataList_i[0].toString();
					QString data2 = allEnvDataList_i[1].toString();
					QTableWidgetItem* item1 = new QTableWidgetItem;
					QTableWidgetItem* item2 = new QTableWidgetItem;
					item1->setText(data1);
					item2->setText(data2);
					table->setItem(i, 0, item1);
					table->setItem(i, 1, item2);
				}
				table->show();
				workbooks->dynamicCall("Close()");
				excel.dynamicCall("Quit()");
				FileChanged = false;
				functionText->setPlainText("Loading excel file successfully!");
			}
		}
	}
}
