#include "myscene.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    num = 13;
}

void MyScene::on_select_line(QImage img)
{
    this->selectLine = true;
    image = img;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectLine)
    {
        QGraphicsScene::mousePressEvent(event);
        setSelectionArea(QPainterPath());
        smthIsSelected = false;

        QPointF pos = event->pos();
        QColor px = image.pixelColor(pos.x(), pos.y());

        if (px == backgroundColorIs(image)) return;

        int i = pos.x();
        int step;
        if (sceneRect().width() > sceneRect().height())
            step = 0.01 * sceneRect().height();
        else step = 0.01 * sceneRect().width();
        bool ptFound = false;

        while ((i < sceneRect().width()) && (step < 0.2 * sceneRect().width()))
        {
            ptFound = false;
            if (!ptFound)
                for (int j = i; j < i + step; j++)
                {
                    if (image.pixelColor(j, pos.y() - step) == px)
                    {
                        QGraphicsItem *it;
                        it = new MyPoint(num++);
                        it->setPos(QPointF(j, pos.y() - step));
                        addItem(it);
                        emit signalPointAdded(items(Qt::AscendingOrder));
                        setSelectionArea(QPainterPath());
                        ptFound = true;
                        i += step;
                        pos = QPointF(j, pos.y() - step);
                        break;
                    }
                }
            if (!ptFound)
                for (int j = pos.y() - step; j < pos.y() + 2 * step; j++)
                {
                    if (image.pixelColor(i + step, j) == px)
                    {
                        QGraphicsItem *it;
                        it = new MyPoint(num++);
                        it->setPos(QPointF(i + step, j));
                        addItem(it);
                        emit signalPointAdded(items(Qt::AscendingOrder));
                        setSelectionArea(QPainterPath());
                        ptFound = true;
                        i += step;
                        pos = QPointF(i + step, j);
                        break;
                    }
                }
            if (!ptFound)
                for (int j = i + step; j > i; j--)
                {
                    if (image.pixelColor(j, pos.y() + step) == px)
                    {
                        QGraphicsItem *it;
                        it = new MyPoint(num++);
                        it->setPos(QPointF(j, pos.y() + step));
                        addItem(it);
                        emit signalPointAdded(items(Qt::AscendingOrder));
                        setSelectionArea(QPainterPath());
                        ptFound = true;
                        i += step;
                        pos = QPointF(j, pos.y() + step);
                        break;
                    }
                }
            if (!ptFound)
                step *= 1.1;
        }
    }

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
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (selectLine)
    {
        selectLine = false;
        return;
    }
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

QColor MyScene::backgroundColorIs(QImage &img)
{
    QColor colors[100];
    int count[100];
    int k = 0, max = 0, where = 0;
    for (int i = 0.1 * img.width(); i < img.width(); i += 0.1 * img.width())
        for (int j = 0.1 * img.height(); j < img.height(); j += 0.1 * img.height())
        {
            colors[k] = img.pixelColor(i, j);
            k++;
        }

    for (int i = 0; i < k; i++)
    {
        count[i]++;
        for (int j = i; j < k; j++)
        {
            if (colors[i] == colors[j])
            {
                count[i]++;
                count[j]++;
                if (count[i] > max)
                {
                    max = count[i];
                    where = i;
                }
            }
        }
    }

    return colors[where];
}
