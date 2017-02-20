#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QPair>
#include <QWaitCondition>
#include <QMutex>
#include <QDir>

class MyThread : public QThread
{
    public:
        MyThread(QQueue <QPair<QString,QString>> &queue, QWaitCondition &empty, QMutex &mutex);

    protected:
        void run();

    private:
        QQueue <QPair<QString,QString>>* DataQueue;
        QWaitCondition* EmptyQueue;
        QMutex* Mutex;
};

#endif // MYTHREAD_H
