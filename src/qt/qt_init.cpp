#include "qt/qt_init.h"
void qt_create_window(int argc, char** argv)
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	a.exec();
}
