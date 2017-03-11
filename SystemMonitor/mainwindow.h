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
#include "lshw.h"
#include "qjsonmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void lshwStart();

private slots:
    void UpdateHardware(QByteArray &Output);

private:
    Ui::MainWindow *ui;
    QQueue <QString> SensorQueue;
    QMutex Smutex;
    MyThread *sthread;

    QTimer fsectimer;
    QTimer stimer;

    QThread lshwThread;
    Lshw lshw_;

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
    void ReadOutput();


};

#endif // MAINWINDOW_H



