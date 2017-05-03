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
    if(myProcess->waitForStarted()){

        myProcess->waitForReadyRead();
        QByteArray aux;
        aux = myProcess->readAllStandardOutput();
        emit readFinished(aux);
    } else {

        QByteArray out;
        out = "Netstat error ";
        out.append(myProcess->errorString());
        emit readFinished(out);
    }
}
