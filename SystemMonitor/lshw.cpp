#include "mainwindow.h"
#include "lshw.h"

Lshw::Lshw()
{
    myProcess = new QProcess(this);
    myProcess->start("/usr/bin/lshw");
}

Lshw::~Lshw()
{
    delete myProcess;
}
