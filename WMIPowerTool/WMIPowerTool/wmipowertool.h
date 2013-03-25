#ifndef WMIPOWERTOOL_H
#define WMIPOWERTOOL_H

#include <QtWidgets/QMainWindow>
#include "ui_wmipowertool.h"

class WMIPowerTool : public QMainWindow
{
	Q_OBJECT

public:
	WMIPowerTool(QWidget *parent = 0);
	~WMIPowerTool();

private:
	Ui::WMIPowerToolClass ui;
};

#endif // WMIPOWERTOOL_H
