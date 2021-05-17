#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QTimer>

#include "server.h"
#include "player.h"
#include "resultrecord.h"

class PeerManager;

class Client : public QObject
{
    Q_OBJECT

public:
    Client(const QString &username);

    QString username;
    QString userstatus;
    QString roomname;
    void sendMessage(const QString &message);
    void sendByteArray(const QByteArray &array);
    void sendMessageTo(const QString &sendercode, const QString &senderIP, const QString &message);
    void sendByteArrayTo(const QString &sendercode, const QString &senderIP, const QByteArray &array);
    void startSendStatus();
    void stopSendStatus();
    QString nickName() const;    
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;
    QList<Player*> playerList;
    QList<Player*> playersInRoom;
    int maxPlayerCount = 3;
    QList<ResultRecord*> result;

signals:
    void newMessage(const QString &from, const QString &message);  //////// ?????
    //void newByteArray(const QString &from, const QByteArray &array);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void updatePlayers();
    void requestConnectToRoom(const QString &from);
    void permissionConnectToRoom(const QString &from, const QString &id);

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();
    void commandDecoder(const QString &from, const QString &message);
    void saveImageToResult(const QString &from, const QByteArray &array);

private:
    void removeConnection(Connection *connection);
    void sendPlayersData();
    QTimer sendStatusTimer;
    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
    bool isPlayerPresent(const QString &usercode, const QString &userIP);
    int getPlayerIdInRoom(const QString &usercode, const QString &userIP);
};

#endif


