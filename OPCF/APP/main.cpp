#include "../WINDOWS/mainwindow.h"
#include <QtWidgets/QApplication>
#include <QFont>
#include "app.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFont font("Microsoft YaHei", 10);
	a.setFont(font);

	app_opcf opcf_a;
	opcf_a.run();

	return a.exec();
}
