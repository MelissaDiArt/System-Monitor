#include "lshw.h"

Lshw::Lshw(QObject *parent) : QObject(parent)
{
    myProcess = new QProcess(this);
    myProcess->start("/usr/bin/lshw -json");
}

Lshw::~Lshw()
{
    delete myProcess;
}

void Lshw::lshwRead()
{
    if(myProcess->waitForStarted()){
        myProcess->waitForReadyRead();
        QByteArray aux;
        aux = myProcess->readAllStandardOutput();
        emit readFinished(aux);

    } else {

        QByteArray out;
        out = "Lshw error: ";
        out.append(myProcess->errorString());
        emit readFinished(out);
    }
}
