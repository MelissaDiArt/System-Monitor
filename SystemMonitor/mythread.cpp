#include "mythread.h"

//Creación del hilo
MyThread::MyThread(QQueue <QPair<QString,QString>> &queue, QWaitCondition &empty, QMutex &mutex)
{
    DataQueue = &queue;
    EmptyQueue = &empty;
    Mutex = &mutex;
}

//Código que ejecuta el hilo
void MyThread::run()
{
    QString path("/sys/class/hwmon");
    QDir* sdir = new QDir(path);


}
