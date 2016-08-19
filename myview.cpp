#include "myview.h"

MyView::MyView(QWidget *parent) : QGraphicsView(parent)
{
    setMouseTracking(true);
}

void MyView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qreal scaleFactor = 1.15;

    if (event->modifiers() == Qt::ControlModifier)
    {
        if (event->delta() > 0)
        {
            scale(scaleFactor, scaleFactor);
            emit signalFromMyView(scaleFactor);
        }
        else
        {
            scale(1/scaleFactor, 1/scaleFactor);
            emit signalFromMyView(1/scaleFactor);
        }
    }
    else QGraphicsView::wheelEvent(event);
}

void MyView::resizeEvent(QResizeEvent *event)
{
        qreal h1 = event->size().height();
        qreal h0 = event->oldSize().height();
        qreal w1 = event->size().width();
        qreal w0 = event->oldSize().width();
        qreal sch, scw;

        if (items().count() != 0)
        {
            scw = sceneRect().width();
            sch = sceneRect().height();

            if (w1 / h1 >= scw / sch)
            {
                scale(h1 / h0, h1 / h0);
                emit signalFromMyView(h1 / h0);
            }
            else
            {
                scale(w1 / w0, w1 / w0);
                emit signalFromMyView(w1 / w0);
            }
        }
}
