#ifndef NETSTAT_H
#define NETSTAT_H


#include <QObject>
#include <QProcess>

class Netstat : public QObject
{
    Q_OBJECT

public:
    explicit Netstat(QObject *parent = 0);
    ~Netstat();

signals:
    void readFinished(QByteArray Output);

public slots:
    void netstatRead();

private:
    QProcess *myProcess;
};

#endif // NETSTAT_H
