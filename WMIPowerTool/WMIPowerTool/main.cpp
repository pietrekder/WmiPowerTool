#include "wmipowertool.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WMIPowerTool w;
	w.show();
	return a.exec();
}
