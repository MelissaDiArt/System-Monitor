#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Sensores
    sthread = new MyThread(SensorQueue,Smutex);
    //Objeto, señal, objeto, slot
    QObject::connect(sthread,&MyThread::QueueEmpty,this,&MainWindow::UpdateSensor);
    QObject::connect(sthread,&MyThread::StartRead,ui->SensorLabel,&QLabel::clear);
    sthread->start();

    //Procesos

     fsectimer.setInterval(5000);
     fsectimer.start();
     QObject::connect(&fsectimer, &QTimer::timeout, this, &MainWindow::UpdateProcess);
     ui->ProcessTreeWidget->sortByColumn(1, Qt::AscendingOrder);

    //Hardware

    qRegisterMetaType<QByteArray>("QByteArray");
    QObject::connect(this,&MainWindow::lshwStart,&lshw_,&Lshw::lshwRead);
    QObject::connect(&lshw_,&Lshw::readFinished,this,&MainWindow::UpdateHardware);
    lshw_.moveToThread(&lshwThread);
    lshwThread.start();
    emit lshwStart();

    //Cpu

    QObject::connect(this, &MainWindow::cpuStart, &cpu_, &Cpu::cpuRead);
    QObject::connect(&cpu_, &Cpu::readFinished, this, &MainWindow::UpdateCpu);
    cpu_.moveToThread(&cpuThread);
    cpuThread.start();
    emit cpuStart();

    //Netstat

    QObject::connect(this,&MainWindow::netstatStart,&netstat_,&Netstat::netstatRead);
    QObject::connect(&netstat_,&Netstat::readFinished,this,&MainWindow::UpdateNetstat);
    netstat_.moveToThread(&netsatThread);
    netsatThread.start();
    emit netstatStart();

    //User

    QObject::connect(this,&MainWindow::sistemUserStart,&sistemUser_,&SistemUser::sistemUserRead);
    QObject::connect(&sistemUser_,&SistemUser::readFinished,this,&MainWindow::UpdateSistemUser);
    sistemUser_.moveToThread(&sistemUserThread);
    sistemUserThread.start();
    emit sistemUserStart();

}

MainWindow::~MainWindow()
{
    delete ui;
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


void MainWindow::UpdateProcess()
{
    QString path("/proc");
    QDir* pdir = new QDir(path);
    QStringList pfilter;
    pfilter << "[0-9]*";
    QStringList pdirectories = pdir->entryList(pfilter);
    delete pdir;

    for(int i=0; i<pdirectories.size();i++)
    {
        QFutureWatcher<Proc> *aux = new QFutureWatcher<Proc>(this);
        QObject::connect(aux, &QFutureWatcher<Proc>::finished, this, &MainWindow::PShow);
        QFuture<Proc> aux1 = QtConcurrent::run(this, &MainWindow::PProperties,path,pdirectories[i]);
        aux->setFuture(aux1);
    }
}


MainWindow::Proc MainWindow::PProperties(QString path, QString ppid)
{
    QString ppath = QString("%1/%2").arg(path).arg(ppid);
    QStringList files;
    files.append(QString("%1/cmdline").arg(ppath));
    files.append(QString("%1/status").arg(ppath));

    Proc result;
    result.PID = ppid;

    for(int i=0; i<files.size(); i++)
    {
        QFile file(files[i]);
        if(file.open(QIODevice::ReadOnly)){

            while(true){

                QByteArray aux = file.readLine();

                if(aux.startsWith("Name:")){

                    result.Name = QString(aux).remove(0,6).remove(result.Name.size()-1,1);

                }else if(aux.startsWith("State:")){

                    result.State = QString(aux).remove(0,7).remove(result.State.size()-1,1);

                }else if(aux.startsWith("PID:")){

                    result.PID = QString(aux).remove(0,6).remove(result.PID.size()-1,1);

                }else if(aux.startsWith("Threads:")){

                    result.Threads = QString(aux).remove(0,9).remove(result.Threads.size()-1,1);

                }else if(aux.startsWith("/")){

                    result.Cmdline = QString(aux);

                }else if(aux == ""){

                    break;
                }
            }
        }
    }

    return result;

}


void MainWindow::PShow()
{

    QFutureWatcher<Proc> *watcher = static_cast< QFutureWatcher<Proc>* >(sender());

    if(watcher){

        QFuture<Proc> future = watcher->future();
        Proc result = future.result();
        QTimer* stimer = new QTimer;
        stimer->setInterval(9990);

        QList<QTreeWidgetItem *> items = ui->ProcessTreeWidget->findItems(result.PID, Qt::MatchFixedString, 1);
        QTreeWidgetItem *process;

        if(!items.isEmpty()){

            process = new QTreeWidgetItem();

        }else{

            process = new QTreeWidgetItem(ui->ProcessTreeWidget);
        }

        QObject::connect(stimer, &QTimer::timeout, this, [=](){

             delete process;
             stimer->deleteLater();
        });

        stimer->start();

        process->setText(0,result.Name);
        process->setText(1,result.PID);

        QTreeWidgetItem *processCmdLine = new QTreeWidgetItem();
        processCmdLine->setText(0,"CmdLine");
        processCmdLine->setText(1,result.Cmdline);

        QTreeWidgetItem *processPID = new QTreeWidgetItem();
        processPID->setText(0,"Parent Process");
        processPID->setText(1,result.PID);

        QTreeWidgetItem *processState = new QTreeWidgetItem();
        processState->setText(0,"State");
        processState->setText(1,result.State);

        QTreeWidgetItem *processThreads = new QTreeWidgetItem();
        processThreads->setText(0,"Threads");
        processThreads->setText(1,result.Threads);

        process->addChild(processCmdLine);
        process->addChild(processPID);
        process->addChild(processState);
        process->addChild(processThreads);

        if(!items.isEmpty()){
            items[0] = process;
        }

        watcher->deleteLater();
    }

}


void MainWindow::UpdateHardware(QByteArray Output)
{
    if(Output.startsWith("Lshw error")){
        QMessageBox::warning(this, tr("Error"), Output , QMessageBox::Ok);
        ui->HardwareTreeView->deleteLater();
    } else {
        QJsonModel* model = new QJsonModel(this);
        model->loadJson(Output);
        ui->HardwareTreeView->setModel(model);
    }
}


void MainWindow::UpdateCpu(QString Output)
{
    if(Output.startsWith("Cpu Info error")){
            QMessageBox::warning(this, "Error", Output , QMessageBox::Ok);
            ui->CPULabel->setText("Can not read CPU info");
        }else{
            ui->CPULabel->setText(Output);
    }
}


void MainWindow::UpdateNetstat(QString Output)
{
    if(Output.startsWith("Netstat error")){
        QMessageBox::warning(this, tr("Error"), Output , QMessageBox::Ok);
        ui->NetstatLabel->setText("Can not read Netstat info");
    } else {
        ui->NetstatLabel->setText(Output);
    }
}

void MainWindow::UpdateSistemUser(QString Output)
{
    if(Output.startsWith("User error")){
        QMessageBox::warning(this, tr("Error"), Output, QMessageBox::Ok);
        ui->UserLabel->setText("Can not read Sistem User");
    } else {
        ui->UserLabel->setText(Output);
    }
}






