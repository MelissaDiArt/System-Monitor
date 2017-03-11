#include "lshw.h"

Lshw::Lshw()
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
    myProcess->waitForReadyRead();
    QByteArray aux;
    aux = myProcess->readAllStandardOutput();
    emit readFinished(aux);
}
