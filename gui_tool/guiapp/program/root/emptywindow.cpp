#include "emptywindow.h"
#include "ui_emptywindow.h"
#include <QStandardItemModel>

EmptyWindow::EmptyWindow(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EmptyWindow)
{
	ui->setupUi(this);
}

EmptyWindow::~EmptyWindow()
{
	delete ui;
}
