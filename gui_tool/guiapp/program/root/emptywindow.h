#ifndef EMPTYWINDOW_H
#define EMPTYWINDOW_H

#include "qdialog.h"

namespace Ui {
class EmptyWindow;
}

class EmptyWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EmptyWindow(QWidget *parent = 0);
    ~EmptyWindow();

protected:
    Ui::EmptyWindow *ui;
};

#endif // EMPTYWINDOW_H
