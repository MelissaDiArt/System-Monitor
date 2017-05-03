#include "sistemuser.h"

SistemUser::SistemUser(QObject *parent) : QObject(parent)
{
    myProcess = new QProcess(this);
    myProcess->start("w -h");
}

SistemUser::~SistemUser()
{
    delete myProcess;
}

void SistemUser::sistemUserRead()
{
    myProcess->waitForReadyRead();
    QByteArray aux;
    aux = myProcess->readAllStandardOutput();
    int aux1 = aux.indexOf("tty");
    while(aux1 != -1)
    {
        int aux2 = aux.indexOf("\n");
        aux.remove(aux1,aux2-aux1);
        aux1 = aux.indexOf("tty");
    }
    emit readFinished(aux);
}
