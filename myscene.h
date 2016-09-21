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

signals:
    void signalFromMyScene(QPointF pos);
    void signalPointAdded(QList<QGraphicsItem *> list);
    void signalDropFromMyScene(QString dropFilename);

public slots:
    void on_select_line(QImage img);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    virtual void keyPressEvent(QKeyEvent *keyEvent);

private:
    QColor backgroundColorIs(QImage& img);

    int num;
    QList<QGraphicsItem *> xy;
    bool smthIsSelected = false, selectLine = false;
    QImage image;
};

#endif // MYSCENE_H
