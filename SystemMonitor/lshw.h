#ifndef LSHW_H
#define LSHW_H

#include <QObject>
#include <QProcess>
#include <QMessageBox>

class Lshw : public QObject
{
    Q_OBJECT

public:
    explicit Lshw(QObject *parent = 0);
    ~Lshw();

signals:
    void readFinished(QByteArray Output);

public slots:
    void lshwRead();

private:
    QProcess *myProcess;
};

#endif // LSHW_H
