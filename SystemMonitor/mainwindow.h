#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QThreadPool>
#include <QFuture>
#include <QFutureWatcher>
#include <QProcess>
#include <QTimer>
#include <QPair>
#include <QtConcurrent/QtConcurrentRun>
#include <QMessageBox>

#include "mythread.h"
#include "lshw.h"
#include "qjsonmodel.h"
#include "cpu.h"
#include "netstat.h"
#include "sistemuser.h"

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
    void cpuStart();
    void netstatStart();
    void sistemUserStart();

private slots:
    void UpdateSensor();
    void UpdateProcess();
    void PShow();
    void UpdateHardware(QByteArray Output);
    void UpdateCpu(QString Output);
    void UpdateNetstat(QString Output);
    void UpdateSistemUser(QString Output);

private:
    Ui::MainWindow *ui;
    QQueue <QString> SensorQueue;
    QMutex Smutex;
    MyThread *sthread;

    QTimer fsectimer;

    QThread lshwThread;
    Lshw lshw_;

    QThread cpuThread;
    Cpu cpu_;

    QThread netsatThread;
    Netstat netstat_;

    QThread sistemUserThread;
    SistemUser sistemUser_;

    struct Proc {
        QString PID;
        QString Cmdline;
        QString Name;
        QString State;
        QString Threads;
    };

    MainWindow::Proc PProperties(QString path, QString ppid);
};

#endif // MAINWINDOW_H



