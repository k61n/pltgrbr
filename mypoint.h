#ifndef MYPOINT_H
#define MYPOINT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

class MyPoint : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)

public:
    MyPoint(int num);
    QRectF boundingRect() const;// Q_DECL_OVERRIDE;
//    QPainterPath shape() const;// Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);// Q_DECL_OVERRIDE;

protected:

private:

};

#endif // MYPOINT_H
