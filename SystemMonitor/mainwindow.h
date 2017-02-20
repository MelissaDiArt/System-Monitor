#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "mythread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QQueue <QPair<QString,QString>> SensorQueue;
    QWaitCondition EmptyQueue;
    QMutex Mutex;


    void UpdateSensor();
};

#endif // MAINWINDOW_H


QQueue <QPair<QString,QString>>* DataQueue;
QWaitCondition* EmptyQueue;
QMutex* Mutex;
