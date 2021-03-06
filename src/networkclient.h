#ifndef __NETWORKCLIENT_H
#define __NETWORKCLIENT_H

#include <memory>
#include <QObject>
#include <QTcpSocket>

struct ADTFMediaSample;

class NetworkClient : public QObject
{
    Q_OBJECT
public:
    explicit NetworkClient(QObject *parent = nullptr);
    QString getPeer();

public slots:
    void connectNetwork(QString host, uint16_t port);
    void disconnectNetwork();
    void send(const ADTFMediaSample &mediaSample);

private slots:
    void receive();
    void error();

signals:
    void connected();
    void disconnected();
    void received(ADTFMediaSample sample);
    void errored(QString errorMsg);

private:
    QTcpSocket tcpSocket;
};

#endif // __NETWORKCLIENT_H
