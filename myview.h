#ifndef MYVIEW_H
#define MYVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include "mypoint.h"
#include "myscene.h"

class MyView : public QGraphicsView
{
    Q_OBJECT

public:
    MyView(QWidget* parent = 0);
   // ~MyView();

signals:
    void signalFromMyView(qreal scaleFactor);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent *event);

private:

};

#endif // MYVIEW_H
