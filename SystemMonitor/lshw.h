#ifndef LSHW_H
#define LSHW_H


class Lshw : public QObject
{

public:
    Lshw();
    ~Lshw();
signals:
    void readFinished(QByteArray &Output);

public slots:
    void lshwRead();
private:
    QProcess *myProcess;
};

#endif // LSHW_H
