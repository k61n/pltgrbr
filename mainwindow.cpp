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
    ui->tabWidget->setCurrentIndex(0);
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
    filename=QFileDialog::getOpenFileName(this, tr("Open file"), ".", "Image files (*.jpg *.bmp *.png *.tif);;All files (*.*)");
    if (filename.isEmpty()) return;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        QMessageBox::information(0, "info", file.errorString());

    initScene();

    image = QImage(filename);
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

void MainWindow::updateList(QList<QGraphicsItem *> list) //updates the table with measured points in user's coordinates if they are set
{                                                       // or in pixels if they aren't, in ascending order based on X
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(list.size() - 13);

    qreal xCoef = 0, yCoef = 0, rCoef = 0, xOffset, yOffset;
    QTableWidgetItem *x, *y;

    if ((setXY) || ((!setXY) && (!setPolar)))   // if XY are set up or if nothing is set up
    {
        if ((!ui->lineEdit->text().isNull()) && (!ui->lineEdit_2->text().isNull()))
        {
            if (!ui->checkBox_X->isChecked())
            {
                xCoef = (ui->lineEdit_2->text().toDouble() - ui->lineEdit->text().toDouble()) / (list.at(3)->pos().x() - list.at(2)->pos().x());
                xOffset = list.at(2)->pos().x() - ui->lineEdit->text().toDouble() / xCoef;
            }
            else
            {
                xCoef = (log10(ui->lineEdit_2->text().toDouble()) - log10(ui->lineEdit->text().toDouble())) / (list.at(3)->pos().x() - list.at(2)->pos().x());
                xOffset = list.at(2)->pos().x() - log10(ui->lineEdit->text().toDouble()) / xCoef;
            }
        }

        if ((!ui->lineEdit_3->text().isNull()) && (!ui->lineEdit_4->text().isNull()))
        {
            if (!ui->checkBox_Y->isChecked())
            {
                yCoef = (ui->lineEdit_4->text().toDouble() - ui->lineEdit_3->text().toDouble()) / (list.at(5)->pos().y() - list.at(4)->pos().y());
                yOffset = list.at(4)->pos().y() - ui->lineEdit_3->text().toDouble() / yCoef;
            }
            else
            {
                yCoef = (log10(ui->lineEdit_4->text().toDouble()) - log10(ui->lineEdit_3->text().toDouble())) / (list.at(5)->pos().y() - list.at(4)->pos().y());
                yOffset = list.at(4)->pos().y() - log10(ui->lineEdit_3->text().toDouble()) / yCoef;
            }
        }
    }

    if (setPolar) //if polar is set up
    {
        if ((!ui->lineEdit_5->text().isNull()) && (!ui->lineEdit_6->text().isNull()))
            rCoef = (ui->lineEdit_6->text().toDouble() - ui->lineEdit_5->text().toDouble()) / (sqrt(pow((list.at(10)->pos().x() - list.at(9)->pos().x()), 2) + pow((list.at(10)->pos().y() - list.at(9)->pos().y()), 2)));
    }

    x = new QTableWidgetItem;
    y = new QTableWidgetItem;

    for (int i = 13; i < list.size(); i++)
    {
        if ((setXY) || ((!setXY) && (!setPolar)))
        {
            if (xCoef == 0)
                x->setData(0, list.at(i)->pos().x());
            else
            {
                if (!ui->checkBox_X->isChecked())
                    x->setData(0, (list.at(i)->pos().x() - xOffset) * xCoef);
                else
                    x->setData(0, pow(10, (list.at(i)->pos().x() - xOffset) * xCoef));
            }
            ui->tableWidget->setItem(i - 13, 0, x);

            if (yCoef == 0)
                y->setData(0, list.at(i)->pos().y());
            else
            {
                if (!ui->checkBox_Y->isChecked())
                    y->setData(0, (list.at(i)->pos().y() - yOffset) * yCoef);
                else
                    y->setData(0, pow(10, (list.at(i)->pos().y() - yOffset) * yCoef));
            }

            ui->tableWidget->setItem(i - 13, 1, y);
        }

        qreal quarter = 0;
        x = new QTableWidgetItem;
        y = new QTableWidgetItem;

        if (setPolar)
        {
            if (i < 13)
            {
                x->setData(0, list.at(i)->pos().x());
                y->setData(0, list.at(i)->pos().y());
            }
            else
            {
                if (rCoef == 0)
                    x->setData(0, sqrt(pow(list.at(i)->pos().x() - list.at(9)->pos().x(), 2) + pow(list.at(i)->pos().y() - list.at(9)->pos().y(), 2)));
                else
                    x->setData(0, rCoef * sqrt(pow(list.at(i)->pos().x() - list.at(9)->pos().x(), 2) + pow(list.at(i)->pos().y() - list.at(9)->pos().y(), 2)));

                if ((list.at(i)->pos().x() >= list.at(9)->pos().x()) && (list.at(i)->pos().y() <= list.at(9)->pos().y())) quarter = 0;
                if ((list.at(i)->pos().x() < list.at(9)->pos().x()) && (list.at(i)->pos().y() <= list.at(9)->pos().y())) quarter = 3.14159265359;
                if ((list.at(i)->pos().x() < list.at(9)->pos().x()) && (list.at(i)->pos().y() > list.at(9)->pos().y())) quarter = 3.14159265359;
                if ((list.at(i)->pos().x() >= list.at(9)->pos().x()) && (list.at(i)->pos().y() > list.at(9)->pos().y())) quarter = 3.14159265359 * 2;

                if (ui->radioButton->isChecked())
                    y->setData(0, quarter + atan((- list.at(i)->pos().y() + list.at(9)->pos().y()) / (list.at(i)->pos().x() - list.at(9)->pos().x())));
                else
                    y->setData(0, qRadiansToDegrees(quarter + atan((- list.at(i)->pos().y() + list.at(9)->pos().y()) / (list.at(i)->pos().x() - list.at(9)->pos().x()))));
            }

            ui->tableWidget->setItem(i - 13, 0, x);
            ui->tableWidget->setItem(i - 13, 1, y);
        }
    }

    if ((setXY) || ((!setXY) && (!setPolar)))
        ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
    if (setPolar)
        ui->tableWidget->sortByColumn(1, Qt::AscendingOrder);
}

void MainWindow::dropFromMyScene(QString dropFilename)
{
    initScene();

    image = QImage(dropFilename);
    filename = dropFilename;

    addImage(image);
}

void MainWindow::on_actionAnalyze_triggered()
{

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
    image = qApp->clipboard()->image();
    if (!image.isNull())
    {
        initScene();
        addImage(image);
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
    connect(this, SIGNAL(signalToSelectLine(QImage)), scene, SLOT(on_select_line(QImage)));

    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
}

void MainWindow::addImage(QImage img)
{
    item = new QGraphicsPixmapItem(QPixmap::fromImage(img));
    scene->addItem(item);

    axes = new MyAxes(scene->sceneRect());
    scene->addItem(axes);
    axes->setEnabled(setXY);
    axes->setVisible(setXY);

    polar = new MyPolar(scene->sceneRect());
    scene->addItem(polar);
    polar->setEnabled(setPolar);
    polar->setVisible(setPolar);

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
    if (ui->graphicsView->scene()->items().count() > 0)
    {
        setXY = !setXY;
        if (setXY)
        {
            ui->tabWidget->setCurrentIndex(1);
            if (setPolar) on_actionSet_polar_plot_triggered();
        }
        else ui->tabWidget->setCurrentIndex(0);
        axes->setEnabled(setXY);
        axes->setVisible(setXY);
    }
}

void MainWindow::on_actionSet_polar_plot_triggered()
{
    if (ui->graphicsView->scene()->items().count() > 0)
    {
        setPolar = !setPolar;
        if (setPolar)
        {
            ui->tabWidget->setCurrentIndex(2);
            if (setXY) on_actionSet_XY_triggered();
        }
        else ui->tabWidget->setCurrentIndex(0);
        polar->setEnabled(setPolar);
        polar->setVisible(setPolar);
    }
}

void MainWindow::on_actionSelect_curve_triggered()
{
    //here i have to emit signal in order to receive then a feedback from my scene
    emit signalToSelectLine(image);
}
