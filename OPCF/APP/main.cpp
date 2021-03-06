#include "../WINDOWS/mainwindow.h"
#include <QtWidgets/QApplication>
#include <QFont>
#include "app.h"

void setUI(QApplication& app);

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFont font("Microsoft YaHei", 10);
	a.setFont(font);

	app_opcf opcf_a;
	opcf_a.run();
	setUI(a);

	return a.exec();
}


void setUI(QApplication& app)
{
	QFile f(":/qdarkstyle/img/src/qdarkstyle/style.qss");
	if (!f.exists())
	{
		qDebug() << ("Unable to set stylesheet, file not found\n") << endl;
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		app.setStyleSheet(ts.readAll());
	}
}



