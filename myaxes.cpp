#include "myaxes.h"

Point::Point(QGraphicsItem *parent) : QGraphicsObject(parent)
{
    setFlags(ItemIsSelectable | ItemIsMovable | ItemIgnoresTransformations | ItemSendsScenePositionChanges);

}

QRectF Point::boundingRect() const
{
    return QRectF(-10, -10, 20, 20);
}

void Point::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QPen pen;
    pen.setColor(Qt::darkMagenta);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(-10, -10, 10, 10);
    painter->drawLine(-10, 10, 10, -10);
}

QVariant Point::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) emit posChanged();
    return QGraphicsItem::itemChange(change, value);
}

MyAxes::MyAxes(QRectF rect)
{
    setFlag(ItemHasNoContents);

    x1 = new Point(this);
    x1->setPos(0.2 * rect.width(), 0.9 * rect.height());

    x2 = new Point(this);
    x2->setPos(0.8 * rect.width(), 0.9 * rect.height());

    y1 = new Point(this);
    y1->setPos(0.1 * rect.width(), 0.2 * rect.height());

    y2 = new Point(this);
    y2->setPos(0.1 * rect.width(), 0.8 * rect.height());

    xAxis = new QGraphicsLineItem(this);
    xAxis->setFlags(ItemIgnoresTransformations);
    yAxis = new QGraphicsLineItem(this);
    yAxis->setFlags(ItemIgnoresTransformations);

    drawAxes();

    connect(x1, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(x2, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(y1, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(y2, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
}

void MyAxes::drawAxes()
{
    QPen pen;
    pen.setColor(Qt::darkMagenta);
    pen.setWidth(3);

    xAxis->setPen(pen);
    xAxis->setLine(x1->pos().x(), x1->pos().y(), x2->pos().x(), x2->pos().y());

    yAxis->setPen(pen);
    yAxis->setLine(y1->pos().x(), y1->pos().y(), y2->pos().x(), y2->pos().y());
}

void MyAxes::on_pos_change()
{
    drawAxes();
}

QRectF MyAxes::boundingRect() const
{
    return QRectF();
}

void MyAxes::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    Q_UNUSED(painter);
}
