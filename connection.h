#ifndef CONNECTION_H
#define CONNECTION_H

#include <QCborStreamReader>
#include <QCborStreamWriter>
#include <QElapsedTimer>
#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

static const int MaxBufferSize = 1024000;

class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        ByteArray,
        Undefined
    };

    Connection(QObject *parent = nullptr);
    Connection(qintptr socketDescriptor, QObject *parent = nullptr);
    ~Connection();

    QString name() const;
    void setGreetingMessage(const QString &message);
    bool sendMessage(const QString &message);
    bool sendByteArray(const QByteArray &array);

signals:
    void readyForUse();
    void newMessage(const QString &from, const QString &message);
    void newByteArray(const QString &from, const QByteArray &array);
protected:
    void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    void processReadyRead();
    void sendPing();
    void sendGreetingMessage();

private:
    bool hasEnoughData();
    void processGreeting();
    void processData();

    QCborStreamReader reader;
    QCborStreamWriter writer;
    QString greetingMessage;
    QString username;
    QTimer pingTimer;
    QElapsedTimer pongTime;
    QString buffer;
    QByteArray bufferArray;
    ConnectionState state;
    DataType currentDataType;
    int transferTimerId;
    bool isGreetingMessageSent;
};

#endif


