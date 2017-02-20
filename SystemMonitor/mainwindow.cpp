#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateSensor()
{
    //Iniciar el nuevo hilo
    MyThread sthread(SensorQueue,EmptyQueue,Mutex);
    sthread.start();

    Mutex.lock();
    if (SensorQueue.size()==0)
    {
        EmptyQueue.wait(&Mutex);
    }
    Mutex.unlock();
    Mutex.lock();
    QString sensordata;
    for(int i=0; i<SensorQueue.size();i++)
    {
        sensordata.append(SensorQueue[i].first).append(" ").append(SensorQueue[i].second);
    }
    ui->SensorLabel->setText(sensordata);
    SensorQueue.clear();
    Mutex.unlock();
    sthread.sleep(1000);
}
