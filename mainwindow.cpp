#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new MyScene();
    ui->graphicsView->setScene(scene);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");

    connect(scene, SIGNAL(signalDropFromMyScene(QString)), this, SLOT(dropFromMyScene(QString))); //allows drag and drop from MyScene
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    filename=QFileDialog::getOpenFileName(this, tr("Open file"), ".", "Image files (*.jpg *.bmp *.png);;All files (*.*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QMessageBox::information(0, "info", file.errorString());

    initScene();

    QImage image(filename);
    file.close();

    addImage(image);
}

void MainWindow::zoomMyScene(QPointF pos)
{
    if (rect.contains(pos))
    {
        a1 = width / 2;
        a2 = width / 2;
        b1 = height / 2;
        b2 = height / 2;
    }
    else
    {
        if (pos.x() + width / 2  > scene->width())
        {
            a2 = scene->width() - pos.x();
            a1 = width - a2;
        }
        else
            if (pos.x() - width / 2 < 0)
            {
                a1 = pos.x();
                a2 = width - a1;
            }

        if (pos.y() + height / 2  > scene->height())
        {
            b2 = scene->height() - pos.y();
            b1 = height - b2;
        }
        else
            if (pos.y() - height / 2 < 0)
            {
                b1 = pos.y();
                b2 = height - b1;
            }
    }

    ui->graphicsView_2->setSceneRect(QRectF(pos - QPointF(a1, b1), pos + QPointF(a2, b2)));
}

void MainWindow::scaleMyScene(qreal scaleFactor)
{
    ui->graphicsView_2->scale(scaleFactor, scaleFactor);
    width /= scaleFactor;
    height /= scaleFactor;

    a1 = width / 2;
    a2 = width / 2;
    b1 = height / 2;
    b2 = height / 2;

    rect = scene->sceneRect();
    rect.adjust(a1, b1, -a2, -b2);
}

void MainWindow::updateList(QList<QGraphicsItem *> list)
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(list.size()-1);

    for (int i = 7; i < list.size(); i++)
    {
        QTableWidgetItem *x = new QTableWidgetItem(QString::number(list.at(i)->pos().x()));
        ui->tableWidget->setItem(i-1, 0, x);
        QTableWidgetItem *y = new QTableWidgetItem(QString::number(list.at(i)->pos().y()));
        ui->tableWidget->setItem(i-1, 1, y);
    }
}

void MainWindow::dropFromMyScene(QString dropFilename)
{
    initScene();

    QImage image(dropFilename);
    filename = dropFilename;

    addImage(image);
}

void MainWindow::on_actionAnalyze_triggered()
{
//    if (filename != NULL)
//    {
//        QList<QGraphicsItem *> list = scene->items(Qt::AscendingOrder);
//        ui->tableWidget->clear();
//        ui->tableWidget->setRowCount(list.size());
//        for (int i = 1; i < list.size(); i++)
//        {
//            ui->tableWidget->setItem(i, 1, QTableWidgetItem(QString::number(list.at(i)->pos.x())));
//            ui->tableWidget->setItem(i, 2, QTableWidgetItem(QString::number(list.at(i)->pos.y())));
////            ui->textEdit->append(QString::number(i) + "\t" + QString::number(list.at(i)->pos().x()) + "\t" + QString::number(list.at(i)->pos().y()));
//        }
//    }
}

void MainWindow::on_actionClose_workspace_triggered()
{
    initScene();
}

void MainWindow::on_actionSave_data_triggered()
{
    QString outputFile = QFileDialog::getSaveFileName(this, tr("Save file"), ".", "Image files (*.txt);;All files (*.*)");
    if (outputFile.isEmpty()) return;

    QFile file(outputFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        QMessageBox::information(0,"info",file.errorString());

    QTextStream outStream(&file);

    for (int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        outStream << ui->tableWidget->item(i, 0)->text() << "\t" << ui->tableWidget->item(i, 1)->text() << "\n";
    }
    file.close();

}

void MainWindow::on_actionExit_program_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionImage_from_clipboard_triggered()
{
    QImage img = qApp->clipboard()->image();
    if (!img.isNull())
    {
        initScene();
        addImage(img);
    }
}

void MainWindow::initScene()
{
    ui->tableWidget->clear();
    scene = new MyScene(this);

    ui->graphicsView->resetTransform();
    ui->graphicsView_2->resetTransform();

    ui->graphicsView->setScene(scene);
    ui->graphicsView_2->setScene(scene);

    connect(scene, SIGNAL(signalFromMyScene(QPointF)), this, SLOT(zoomMyScene(QPointF))); //connects mouseMoveEvents in MyScene with magnifier
    connect(ui->graphicsView, SIGNAL(signalFromMyView(qreal)), this, SLOT(scaleMyScene(qreal))); //scales magnifier as it happens with MyScene
    connect(scene, SIGNAL(signalPointAdded(QList<QGraphicsItem*>)), this, SLOT(updateList(QList<QGraphicsItem*>))); //when new MyPoint is added sends event to textEdit
    connect(scene, SIGNAL(signalDropFromMyScene(QString)), this, SLOT(dropFromMyScene(QString))); //allows drag n drop from MyScene
    connect(this, SIGNAL(signalToSetXY()), scene, SLOT(on_set_XY())); //appear disappear axes

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
}

void MainWindow::addImage(QImage image)
{
    item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    scene->addItem(item);

    axes = new MyAxes(scene->sceneRect());
    scene->addItem(axes);
    axes->setEnabled(setXY);
    axes->setVisible(setXY);


    qreal h1 = ui->graphicsView->height();
    qreal h2 = image.height();
    qreal ratio = 0.99 * h1/h2;
    ui->graphicsView->scale(ratio, ratio);

    width = ui->graphicsView_2->width();
    height = ui->graphicsView_2->height();
    a1 = width / 2;
    a2 = width / 2;
    b1 = height / 2;
    b2 = height / 2;

    rect = scene->sceneRect();
    rect.adjust(a1, b1, -a2, -b2);
}

void MainWindow::on_actionSet_XY_triggered()
{
    // have to make signal to MyScene and have to make axis class
//    emit signalToSetXY();
    setXY = !setXY;
    axes->setEnabled(setXY);
    axes->setVisible(setXY);
}

void MainWindow::on_actionSet_polar_plot_triggered()
{
    emit signalToSetXY();
}
