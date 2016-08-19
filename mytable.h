#ifndef MYTABLE_H
#define MYTABLE_H

#include <QTableWidget>
#include <QWidget>
#include <QKeyEvent>

class MyTable : public QTableWidget
{
    Q_OBJECT

public:
    MyTable(QWidget *parent = 0);

private:
    void copy();

protected:
    virtual void keyPressEvent(QKeyEvent *event);
};

#endif // MYTABLE_H
