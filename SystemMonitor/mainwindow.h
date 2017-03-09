#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThreadPool>
#include <QFuture>
#include <QFutureWatcher>
#include <QProcess>
#include <QTimer>
#include <QtConcurrent/QtConcurrentRun>

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
    QQueue <QString> SensorQueue;
    QMutex Smutex;
    MyThread *sthread;

    QTimer fsectimer;
    QTimer stimer;

    struct Proc {
        QString PID;
        QString Cmdline;
        QString Name;
        QString State;
        QString Threads;
    };


    void UpdateSensor();
    void UpdateProcess();
    MainWindow::Proc PProperties(QString path, QString ppid);
    void PShow();


};

#endif // MAINWINDOW_H



