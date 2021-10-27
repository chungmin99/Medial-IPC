#pragma once

#include <QtWidgets/QMainWindow>
#include <QToolButton>
#include "Ui/BaseMainWidget.h"
#include "ui_SimFramework.h"
class SimFramework : public QMainWindow
{
	Q_OBJECT

public:
	SimFramework(QWidget *parent = Q_NULLPTR);
private:
	Ui::SimFrameworkClass _ui;
};
