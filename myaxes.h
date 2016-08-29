#ifndef MYAXES_H
#define MYAXES_H

#include <QtWidgets>
#include <QtCore>
#include <QPainter>

class Point : public QGraphicsObject
{
    Q_OBJECT
public:
    Point(QGraphicsItem *parent = 0);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

signals:
    void posChanged();

protected:
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

class MyAxes : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    MyAxes(QRectF rec);

    void drawAxes();

    struct Intrsct{
        bool areIntersected;
        QPointF inPoint;
    };

    Intrsct intersection(QLineF a, QLineF b);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

public slots:
    void on_pos_change();

private:
    QRectF rect;
    QGraphicsLineItem *xAxis, *yAxis;
    QGraphicsObject *x1, *x2, *y1, *y2;
};

class MyPolar : public QObject, public QGraphicsItem
{
        Q_OBJECT
        Q_INTERFACES(QGraphicsItem)
public:
    MyPolar(QRectF rec);

    void drawAxes();

    struct Intrsct{
        bool areIntersected;
        QPointF inPoint;
    };

    Intrsct intersection(QLineF a, QLineF b);

    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) Q_DECL_OVERRIDE;

public slots:
    void on_pos_change();

private:
    QRectF rect;
    QGraphicsObject *x1, *x2;
    QGraphicsLineItem *xAxis;
    QGraphicsEllipseItem *circle;
};

#endif // MYAXES_H
