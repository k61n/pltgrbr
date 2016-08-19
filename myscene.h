#ifndef MYSCENE_H
#define MYSCENE_H

#include <QtCore>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>
#include "mypoint.h"
#include "myaxes.h"

class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyScene(QObject *parent = 0);    

public slots:
    void on_set_XY();

signals:
    void signalFromMyScene(QPointF pos);
    void signalPointAdded(QList<QGraphicsItem *> list);
    void signalDropFromMyScene(QString dropFilename);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;

private:
    void setUpXY();

    int num;
    int j = 0;
    QList<QGraphicsItem *> xy; //x1, x2, y1, y2
};

#endif // MYSCENE_H
