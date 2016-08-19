#include "mytable.h"
#include <QClipboard>
#include <QApplication>

MyTable::MyTable(QWidget *parent) : QTableWidget(parent)
{
//    setFocusPolicy(Qt::ClickFocus);
}

void MyTable::copy()
{
    QString textToClipboard;
    QModelIndexList ind = selectionModel()->selectedIndexes();

    for (int i =0; i < ind.count(); i++)
    {
        textToClipboard += ind.at(i).data().toString();
        if (i + 1 < ind.count())
        {
            if (ind.at(i).row() == ind.at(i+1).row()) textToClipboard += "\t";
            else textToClipboard += "\n";
        }
        else textToClipboard += "\n";
    }

    qApp->clipboard()->setText(textToClipboard, QClipboard::Clipboard);
}

void MyTable::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        copy();
    }
    else
    {
        QTableWidget::keyPressEvent(event);
    }
}
