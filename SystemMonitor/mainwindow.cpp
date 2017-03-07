#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Crear un hilo
    sthread = new MyThread(SensorQueue,Smutex);
    QObject::connect(sthread,&MyThread::QueueEmpty,this,&MainWindow::UpdateSensor);
    sthread->start();

}

MainWindow::~MainWindow()
{
    delete ui;
    sthread->wait();
    delete sthread;
}

void MainWindow::UpdateSensor()
{

    Smutex.lock();
    QString sensordata = ui->SensorLabel->text();
    for(int i=0; i<SensorQueue.size();i++)
    {
       sensordata.append(SensorQueue[i]);
    }
    ui->SensorLabel->setText(sensordata);
    SensorQueue.clear();
    Smutex.unlock();

}

