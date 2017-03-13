#include "mythread.h"

//Creación del hilo
MyThread::MyThread(QQueue <QString> &queue, QMutex &mutex)
{
    DataQueue = &queue;
    Mutex = &mutex;
}

//Código que ejecuta el hilo
void MyThread::run()
{
    while(!false){

        emit StartRead();
        QString path("/sys/class/hwmon");
        QStringList sfilter; //Lista de filtros
        sfilter << "hwmon*";
        QDir* sdir = new QDir(path);
        QStringList sdirectories = sdir->entryList(sfilter); //Lista de directorios
        delete sdir;

        for (int i=0;i<sdirectories.size();i++)
        {
            QString sdirectoriespath = QString("%1/%2").arg(path).arg(sdirectories[i]);
            QDir* eachsdiresctories = new QDir(sdirectoriespath);
            sfilter.clear();
            sfilter << "temp*" << "fan*" << "pwn*" << "in*" << "cpu*" << "curr*" << "power*";
            QStringList files = eachsdiresctories->entryList(sfilter);
            QString namepath = QString("%1/%2").arg(sdirectoriespath).arg("name");
            QFile file(namepath);
            QString name;
            if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                QByteArray line = file.readAll();
                name = QString(line.remove(line.size()-1,2));
                name.append(":\n");
            }

            Mutex->lock();
            DataQueue->append(name);
            Mutex->unlock();
            delete eachsdiresctories;

            for(int j=0; j<files.size(); j++)
            {
                QString filepath = QString("%1/%2").arg(sdirectoriespath).arg(files[j]);
                QFile file(filepath);

                if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
                    QByteArray Sline = file.readAll();
                    QString line(Sline);
                    if(!Sline.endsWith("\n")){
                        line.append("\n");
                    }

                    QString aux = QString("\t %1 = %2").arg(files[j]).arg(line);

                    Mutex->lock();
                    DataQueue->append(aux);
                    Mutex->unlock();
                }

            }

            emit QueueEmpty();
        }

        this->sleep(1);
    }

}






