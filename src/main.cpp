#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	MainWindow mainWindow;
	if(argc > 1)
		mainWindow.withFile(argv[1]);
	mainWindow.show();
	return app.exec();
}