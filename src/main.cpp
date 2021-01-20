#include "mainnoter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainNoter w;

	//QSettings settings("NoterCompany", QCoreApplication::applicationName());
	w.darkThemeEnabled = w.settings.value("darkThemeEnabled", false).toBool();

	QString styleSheet;

	if (w.darkThemeEnabled)
	{
		QFile file(":/theme/style/darkTheme.qss");
		file.open(QFile::ReadOnly);
		styleSheet = file.readAll();
	}
	else
		styleSheet = "";

	qApp->setStyleSheet(styleSheet);

	w.show();
	return a.exec();
}
