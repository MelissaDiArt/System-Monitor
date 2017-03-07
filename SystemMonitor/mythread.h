#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QThread>
#include <QQueue>
#include <QPair>
#include <QWaitCondition>
#include <QMutex>
#include <QDir>
#include <QFile>


class MyThread : public QThread
{
    Q_OBJECT

    public:
        MyThread(QQueue <QString> &queue, QMutex &mutex);

    protected:
        void run();

    signals:
        void QueueEmpty();

    private:
        QQueue <QString>* DataQueue;
        QMutex* Mutex;
};

#endif // MYTHREAD_H
