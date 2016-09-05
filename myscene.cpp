#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    num = 13;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (sceneRect().contains(event->scenePos()))
    {
        QGraphicsScene::mousePressEvent(event);
        if (mouseGrabberItem() == 0)
        {
            setSelectionArea(QPainterPath());
            smthIsSelected = false;
        }
        else
            smthIsSelected = true;
    }
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) //is connected with magnifier
{
    if (sceneRect().contains(event->scenePos()))
    {
        QGraphicsScene::mouseMoveEvent(event);
        emit signalFromMyScene(event->scenePos());
        emit signalPointAdded(items(Qt::AscendingOrder));
    }
    else return;
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (sceneRect().contains(event->scenePos()))
    {
        QGraphicsScene::mouseReleaseEvent(event);
        if (!smthIsSelected)
        {
            if ((event->button() == Qt::LeftButton) && (selectionArea().boundingRect().isNull()))
            {
                QGraphicsItem *it;
                it = new MyPoint(num++);
                it->setPos(event->scenePos());
                addItem(it);
                emit signalPointAdded(items(Qt::AscendingOrder));
                setSelectionArea(QPainterPath());
            }
        }
        else
            emit signalPointAdded(items(Qt::AscendingOrder));
    }
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
        QGraphicsScene::keyPressEvent(keyEvent);
}
