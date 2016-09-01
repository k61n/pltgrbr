#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    num = 6;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (sceneRect().contains(event->scenePos()))
    {
        QGraphicsScene::mousePressEvent(event);
        if (mouseGrabberItem() == 0)
            if (event->button() == Qt::LeftButton)
            {
                QGraphicsItem *it;
                it = new MyPoint(num++);
                it->setPos(event->scenePos());
                addItem(it);
            }
    }
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) //is connected with magnifier
{
    if (sceneRect().contains(event->scenePos()))
    {
        QGraphicsScene::mouseMoveEvent(event);
        emit signalFromMyScene(event->scenePos());
    }
    else return;
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit signalPointAdded(items(Qt::AscendingOrder));
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    emit signalDropFromMyScene(event->mimeData()->urls().at(0).toLocalFile());
}

void MyScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if (event->mimeData()->urls().count() != 1)
    {
        event->setAccepted(false);
    }
    else
    {
        QString fn = event->mimeData()->urls().at(0).toLocalFile();
        QByteArray format = QImageReader::imageFormat(fn);

        if (!format.isEmpty())
            event->setAccepted(true);
        else event->setAccepted(false);
    }
}

void MyScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void MyScene::keyPressEvent(QKeyEvent *keyEvent)
{
    if (keyEvent->matches(QKeySequence::Delete))
        for (int i = selectedItems().count() - 1; i >= 0; i--)
            removeItem(selectedItems().at(i));
    else
        QGraphicsScene::keyReleaseEvent(keyEvent);
}
