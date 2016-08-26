
#pragma once

#include "qtreeview.h"
#include "qevent.h"
#include "qstandarditemmodel.h"
#include "qcombobox.h"
#include "qheaderview.h"
#include "qlistview.h"
#include "qstringlistmodel.h"

class TreeBox : public QComboBox 
{ 
public:
       TreeBox(QWidget* parent = 0) : QComboBox(parent), skipNextHide(false)
       {
		QTreeView* vw =	new QTreeView(this);
		vw->header()->hide();
		setView(vw);
		view()->viewport()->installEventFilter(this);               
       }

       bool eventFilter(QObject* object, QEvent* event)
       {
		if (event->type() == QEvent::MouseButtonRelease && object == view()->viewport())
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event); 
			QModelIndex index = view()->indexAt(mouseEvent->pos());
			if (!view()->visualRect(index).contains(mouseEvent->pos()))
			skipNextHide = true;
		}
		return false;
       }

       virtual void showPopup()
       {
		QComboBox::showPopup();
       }

       virtual void hidePopup()
       {               
		setCurrentIndex(view()->currentIndex().row());
		if (skipNextHide)
			skipNextHide = false;
		else
			QComboBox::hidePopup();
       }

private:
       bool skipNextHide;
};
