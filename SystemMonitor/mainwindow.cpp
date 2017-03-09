#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Sensores
    sthread = new MyThread(SensorQueue,Smutex);
    QObject::connect(sthread,&MyThread::QueueEmpty,this,&MainWindow::UpdateSensor);
    QObject::connect(sthread,&MyThread::StartRead,ui->SensorLabel,&QLabel::clear);
    sthread->start();

    //Procesos

    fsectimer.start();
    stimer.setInterval(4000);

    QObject::connect(&fsectimer, &QTimer::timeout, this, &MainWindow::UpdateProcess);
    ui->ProcessTreeWidget->sortByColumn(1, Qt::AscendingOrder);


    //Hardware


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

void MainWindow::UpdateProcess()
{
    QString path("/proc");
    QDir* pdir = new QDir(path);
    QStringList pfilter;
    pfilter << "[0-9]*";
    QStringList pdirectories = pdir->entryList(pfilter);

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

                //    aux.replace('\000',"");
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

        QList<QTreeWidgetItem *> items = ui->ProcessTreeWidget->findItems(result.PID, Qt::MatchFixedString, 1);
        QTreeWidgetItem *process;

        if(!items.isEmpty()){
            process = new QTreeWidgetItem();
        }else{
            process = new QTreeWidgetItem(ui->ProcessTreeWidget);
        }

        QObject::connect(&stimer, &QTimer::timeout, this, [=](){
             delete process;
        });

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






