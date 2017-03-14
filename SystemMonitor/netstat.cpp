#include "netstat.h"

Netstat::Netstat(QObject *parent) : QObject(parent)
{
    myProcess = new QProcess(this);
    myProcess->start("/bin/netstat -tu"); //TCP y UDP
}

Netstat::~Netstat()
{
    delete myProcess;
}

void Netstat::netstatRead()
{
    myProcess->waitForReadyRead();
    QByteArray aux;
    aux = myProcess->readAllStandardOutput();
    emit readFinished(aux);
}
