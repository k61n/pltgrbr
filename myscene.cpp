#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    num = 6;
//    axes = new MyAxes(sceneRect());
//    addItem(axes);
//    axes->setEnabled(setXY);
//    axes->setVisible(setXY);
}

void MyScene::on_set_XY()
{
    qDebug() << "signal received";
//    setXY = !setXY;
//    axes->setEnabled(setXY);
//    axes->setVisible(setXY);
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
//    if (!setXY)
        if (event->button() == Qt::LeftButton)
        {
            emit signalPointAdded(items(Qt::AscendingOrder));
        }
    QGraphicsScene::mouseReleaseEvent(event);
}

void MyScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
//    event->accept();
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
        QString fn = event->mimeData()->urls().at(0).fileName(QUrl::FullyDecoded);
        QString ext = "";
        int i = fn.size()-1;

        while (i >= 0)
        {
            if (fn.at(i) != '.')
            {
                ext = fn.at(i) + ext;
                i--;
            }
            else break;
        }

        if ((ext == "png") || (ext == "jpg") || (ext == "jpeg") || (ext == "bmp")
                || (ext == "PNG") || (ext == "JPG") || (ext == "JPEG") || (ext == "BMP"))
            event->setAccepted(true);
        else event->setAccepted(false);
    }
}

void MyScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    event->accept();
}

void MyScene::setUpXY()
{
//    QGraphicsItem *it;
//    axes = new QGraphicsItemGroup;
//    it = new MyAxes(1);
//    it->setPos(0.2 * sceneRect().width(), 0.9 * sceneRect().height());
//    xy.push_back(it);
//    axes->addToGroup(it);
//    addItem(it);

//    it = new MyAxes(2);
//    it->setPos(0.8 * sceneRect().width(), 0.9 * sceneRect().height());
//    xy.push_back(it);
//    axes->addToGroup(it);
//    addItem(it);

//    it = new MyAxes(3);
//    it->setPos(0.1 * sceneRect().width(), 0.2 * sceneRect().height());
//    xy.push_back(it);
//    axes->addToGroup(it);
//    addItem(it);

//    it = new MyAxes(4);
//    it->setPos(0.1 * sceneRect().width(), 0.8 * sceneRect().height());
//    xy.push_back(it);
//    axes->addToGroup(it);
//    addItem(it);

//    QPen pen;
//    pen.setWidth(5);
//    pen.setColor(Qt::darkMagenta);

//    QGraphicsLineItem *line = new QGraphicsLineItem;
//    line->setPen(pen);
//    line->setLine(xy.at(0)->pos().x(), xy.at(0)->pos().y(), xy.at(1)->pos().x(), xy.at(1)->pos().y());
//    line->setZValue(5);
//    axes->addToGroup(line);
//    addItem(line);

//    QGraphicsLineItem *line1 = new QGraphicsLineItem;
//    line1->setPen(pen);
//    line1->setLine(xy.at(2)->pos().x(), xy.at(2)->pos().y(), xy.at(3)->pos().x(), xy.at(3)->pos().y());
//    line1->setZValue(6);
//    axes->addToGroup(line1);
//    addItem(line1);
}
