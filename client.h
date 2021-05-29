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
    int userId = -1;
    void sendMessage(const QString &message);
    void sendByteArray(const QByteArray &array);
    void sendMessageTo(const QString &sendercode, const QString &senderIP, const QString &message);
    void sendByteArrayTo(const QString &sendercode, const QString &senderIP, const QByteArray &array);
    void startSendStatus();
    void stopSendStatus();
    QString nickName() const;       ///////// ??? del
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;
    QList<Player*> playerList;
    QList<Player*> playersInRoom;
    int maxPlayerCount = 3;
    int currentGameStep = 0;
    bool isPlayGame = false;
    QList<ResultRecord*> result;
    ResultRecord* getFromResult(int step, int id);
    QByteArray commandToByteArray(const QString &command, const QByteArray &array);

signals:
    void newMessage(const QString &from, const QString &message);  //////// ????? del
    void newParticipant(const QString &nick);  /////////////??????? del
    void participantLeft(const QString &nick); ////////////?????
    void updatePlayers();
    void requestConnectToRoom(const QString &from);
    void permissionConnectToRoom(const QString &from, const QString &id);
    void deleteRoom();
    void startGame();
    void nextGameStep();
    void endGame();
    void setGameStep(const QString &gameStep);
    void gameShowImage(const QByteArray &array);
    void gameShowMessage(const QString &message);
    void disconnectFromRoom(const QString &from);
    void stopGameError(const QString &message);

private slots:
    void newConnection(Connection *connection);
    void connectionError(QAbstractSocket::SocketError socketError);
    void disconnected();
    void readyForUse();
    void commandMessageDecoder(const QString &from, const QString &message);
    void commandByteArrayDecoder(const QString &from, const QByteArray &array);
    void saveImageToResult(const QByteArray &array, const int &step, const int &id);
    void saveMessageToResult(const QString &message, const int &step, const int &id);

private:
    void removeConnection(Connection *connection);
    void sendPlayersData();
    QTimer sendStatusTimer;
    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
    bool isPlayerPresent(const QString &usercode, const QString &userIP);
    bool isPlayerInRoomPresent(const QString &user);
    QString byteArrayToCommand(const QByteArray &array);
    bool isMyRoom(const QString &from);
    void removeFromLists(const QString &playername);
};

#endif


