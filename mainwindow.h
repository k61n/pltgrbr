#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "myscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void zoomMyScene(QPointF pos);
    void scaleMyScene(qreal scaleFactor);
    void updateList(QList<QGraphicsItem *> list);
    void dropFromMyScene(QString dropFilename); //drop to MyScene

signals:
    void signalToSetXY();

private slots:
    void on_actionOpen_file_triggered();
    void on_actionAnalyze_triggered();
    void on_actionClose_workspace_triggered();
    void on_actionSave_data_triggered();
    void on_actionExit_program_triggered();
    void on_actionImage_from_clipboard_triggered();

    void on_actionSet_XY_triggered();

    void on_actionSet_polar_plot_triggered();

private:
    void initScene();
    void addImage(QImage image);

    MyScene *scene;
    QGraphicsItem *item;    
    QGraphicsItem *axes;
    bool setXY = false;
    QString filename;
    Ui::MainWindow *ui;

    qreal a1, a2, b1, b2, width, height;
    QRectF rect;
};

#endif // MAINWINDOW_H
