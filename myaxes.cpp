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
    pen.setColor(Qt::cyan);
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

MyAxes::MyAxes(QRectF rec)
{
    setFlag(ItemHasNoContents);

    this->rect = rec;

    x1 = new Point(this);
    x1->setPos(0.2 * rect.width(), 0.9 * rect.height());

    x2 = new Point(this);
    x2->setPos(0.8 * rect.width(), 0.9 * rect.height());

    y1 = new Point(this);
    y1->setPos(0.1 * rect.width(), 0.8 * rect.height());

    y2 = new Point(this);
    y2->setPos(0.1 * rect.width(), 0.2 * rect.height());

    xAxis = new QGraphicsLineItem(this);
    xAxis->setOpacity(0.5);
//    xAxis->setFlags(ItemIgnoresTransformations);
    yAxis = new QGraphicsLineItem(this);
    yAxis->setOpacity(0.5);
//    yAxis->setFlags(ItemIgnoresTransformations);

    drawAxes();

    connect(x1, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(x2, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(y1, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
    connect(y2, SIGNAL(posChanged()), this, SLOT(on_pos_change()));
}

void MyAxes::drawAxes()
{
    QPen pen;
    pen.setColor(Qt::cyan);
    pen.setWidth(5);

    QPointF px1, px2, py1, py2;
    Intrsct test;
    int i = 0;

    xAxis->setPen(pen);

    if (x1->pos().x() != x2->pos().x())
    {
        test = intersection(QLineF(x1->pos(), x2->pos()), QLineF(0, 0, rect.width(), 0));
        if (test.areIntersected)
            if ((test.inPoint.x() >= 0) && (test.inPoint.x() <= rect.width()))
            {
                px1 = test.inPoint;
                i++;
            }

        test = intersection(QLineF(x1->pos(), x2->pos()), QLineF(0, 0, 0, rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.y() >= 0) && (test.inPoint.y() <= rect.height()))
            {
                if (i == 0)
                {
                    px1 = test.inPoint;
                    i++;
                }
                else
                {
                    px2 = test.inPoint;
                }
            }

        test = intersection(QLineF(x1->pos(), x2->pos()), QLineF(0, rect.height(), rect.width(), rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.x() >= 0) && (test.inPoint.x() <= rect.width()))
            {
                if (i == 0)
                {
                    px1 = test.inPoint;
                    i++;
                }
                else
                {
                    px2 = test.inPoint;
                }
            }

        test = intersection(QLineF(x1->pos(), x2->pos()), QLineF(rect.width(), 0, rect.width(), rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.y() >= 0) && (test.inPoint.y() <= rect.height()))
            {
                if (i == 0)
                {
                    px1 = test.inPoint;
                    i++;
                }
                else
                {
                    px2 = test.inPoint;
                }
            }

        xAxis->setLine(px1.x(), px1.y(), px2.x(), px2.y());
    }
    else
    {
        xAxis->setLine(x1->pos().x(), 0, x1->pos().x(), rect.height());
    }

    i = 0;
    yAxis->setPen(pen);

    if (y1->pos().x() != y2->pos().x())
    {
        test = intersection(QLineF(y1->pos(), y2->pos()), QLineF(0, 0, rect.width(), 0));
        if (test.areIntersected)
            if ((test.inPoint.x() >= 0) && (test.inPoint.x() <= rect.width()))
            {
                py1 = test.inPoint;
                i++;
            }

        test = intersection(QLineF(y1->pos(), y2->pos()), QLineF(0, 0, 0, rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.y() >= 0) && (test.inPoint.y() <= rect.height()))
            {
                if (i == 0)
                {
                    py1 = test.inPoint;
                    i++;
                }
                else
                {
                    py2 = test.inPoint;
                }
            }

        test = intersection(QLineF(y1->pos(), y2->pos()), QLineF(0, rect.height(), rect.width(), rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.x() >= 0) && (test.inPoint.x() <= rect.width()))
            {
                if (i == 0)
                {
                    py1 = test.inPoint;
                    i++;
                }
                else
                {
                    py2 = test.inPoint;
                }
            }

        test = intersection(QLineF(y1->pos(), y2->pos()), QLineF(rect.width(), 0, rect.width(), rect.height()));
        if (test.areIntersected)
            if ((test.inPoint.y() >= 0) && (test.inPoint.y() <= rect.height()))
            {
                if (i == 0)
                {
                    py1 = test.inPoint;
                    i++;
                }
                else
                {
                    py2 = test.inPoint;
                }
            }

        yAxis->setLine(py1.x(), py1.y(), py2.x(), py2.y());
    }
    else
    {
        yAxis->setLine(y1->pos().x(), 0, y1->pos().x(), rect.height());
    }
}

MyAxes::Intrsct MyAxes::intersection(QLineF a, QLineF b)
{
    qreal ma, mb, ca, cb;
    Intrsct result;

    if (!((a.x1() == a.x2()) && (b.x1() == b.x2())))
    {
        if ((a.x1() == a.x2()) && (b.x1() != b.x2()))
        {
            mb = (b.y2() - b.y1()) / (b.x2() - b.x1());
            cb = b.y2() - mb * b.x2();
            result.inPoint = QPointF(a.x1(), mb * a.x1() + cb);
            result.areIntersected = true;
        }
        else
        {
            if ((a.x1() != a.x2()) && (b.x1() == b.x2()))
            {
                ma = (a.y2() - a.y1()) / (a.x2() - a.x1());
                ca = a.y1() - ma * a.x1();
                result.inPoint = QPointF(b.x2(), ma * b.x2() + ca);
                result.areIntersected = true;
            }
            else
            {
                ma = (a.y2() - a.y1()) / (a.x2() - a.x1());
                ca = a.y1() - ma * a.x1();
                mb = (b.y2() - b.y1()) / (b.x2() - b.x1());
                cb = b.y2() - mb * b.x2();
                if (ma != mb)
                {
                    result.inPoint = QPointF((cb - ca) / (ma - mb), mb * (cb - ca) / (ma - mb) + cb);
                    result.areIntersected = true;
                }
                else
                {
                    result.inPoint = QPointF(0, 0);
                    result.areIntersected = false;
                }
            }
        }
    }
    else
    {
        result.inPoint = QPointF(0, 0);
        result.areIntersected = false;
    }

    return result;
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
