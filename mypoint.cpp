#include "mypoint.h"
#include <QStyleOptionGraphicsItem>

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

void MyPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen;
    pen.setColor(Qt::magenta);
    pen.setWidth(2);

    if (isSelected())
        pen.setStyle(Qt::DotLine);
    else
        pen.setStyle(Qt::SolidLine);

    painter->setPen(pen);

    painter->drawRect(QRectF(QPointF(-6, -6), QPointF(6, 6)));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawLine(-10, 0, 10, 0);
    painter->drawLine(0, -10, 0, 10);
}

void MyPoint::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setOpacity(0.5);
    QGraphicsItem::update();
}

void MyPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    setOpacity(1);
    QGraphicsItem::update();
}

void MyPoint::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    Q_UNUSED(event);
    QGraphicsItem::update();
}
