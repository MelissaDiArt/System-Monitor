#include "cpu.h"

Cpu::Cpu(QObject *parent) : QObject(parent)
{
    file = new QFile("/proc/cpuinfo");
}

Cpu::~Cpu()
{
    delete file;
}

void Cpu::cpuRead()
{
    QByteArray output;
    if(file->open(QIODevice::ReadOnly)){

    output = file->readAll();
    emit readFinished(output);
    }
}
