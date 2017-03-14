#ifndef SISTEMUSER_H
#define SISTEMUSER_H


#include <QObject>
#include <QProcess>

class SistemUser : public QObject
{
    Q_OBJECT

public:
    explicit SistemUser(QObject *parent = 0);
    ~SistemUser();

signals:
    void readFinished(QByteArray Output);

public slots:
    void sistemUserRead();

private:
    QProcess *myProcess;
};

#endif // SISTEMUSER_H
