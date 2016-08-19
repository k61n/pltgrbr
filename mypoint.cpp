#include "mypoint.h"

MyPoint::MyPoint(int num)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations);
    setAcceptHoverEvents(true);
    setZValue(num);
}

QRectF MyPoint::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

//QPainterPath MyPoint::shape() const
//{
//    QPainterPath path;
//    path.addRect(-10, -10, 20, 20);
//    return path;
//}

void MyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen;
    pen.setColor(Qt::magenta);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawRect(QRectF(QPointF(-6, -6), QPointF(6, 6)));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(-10, 0, 10, 0);
    painter->drawLine(0, -10, 0, 10);
}
