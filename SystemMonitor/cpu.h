#ifndef CPU_H
#define CPU_H

#include <QObject>
#include <QProcess>
#include <QFile>
#include <QMessageBox>

class Cpu : public QObject
{
    Q_OBJECT

public:
    explicit Cpu(QObject *parent = 0);
    ~Cpu();

signals:
    void readFinished(QByteArray Output);

public slots:
    void cpuRead();

private:
    QFile *file;
};

#endif // CPU_H
