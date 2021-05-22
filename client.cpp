#include <QtNetwork>

#include "client.h"
#include "connection.h"
#include "peermanager.h"

static const qint32 sendStatusInterval = 2000;

Client::Client(const QString &username)
{
    this->username = username;
    peerManager = new PeerManager(this);
    peerManager->setServerPort(server.serverPort());
    peerManager->startBroadcasting();

    sendStatusTimer.setInterval(sendStatusInterval);
    connect (&sendStatusTimer, &QTimer::timeout, this, &Client::sendPlayersData);

    connect(peerManager, &PeerManager::newConnection, this, &Client::newConnection);
    connect(&server, &Server::newConnection, this, &Client::newConnection);
}

void Client::sendMessage(const QString &message)
{
    if (message.isEmpty())
        return;

    for (Connection *connection : qAsConst(peers))
        if (connection->localPort() == server.serverPort()) {
            connection->sendMessage(message);
        }
}

void Client::sendByteArray(const QByteArray &array) // ++++
{
    if (array.isEmpty())
        return;

    for (Connection *connection : qAsConst(peers))
        if (connection->localPort() == server.serverPort()) {
            connection->sendByteArray(array);
        }
}


QString Client::nickName() const
{
    return peerManager->userName();
    //return peerManager->userName() + '@' + QHostInfo::localHostName() + ':' + QString::number(server.serverPort());
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return peers.contains(senderIp);

    if (!peers.contains(senderIp))
        return false;

    const QList<Connection *> connections = peers.values(senderIp);
    for (const Connection *connection : connections) {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}

void Client::newConnection(Connection *connection)
{
    connection->setGreetingMessage(peerManager->userName());

    connect(connection, &Connection::errorOccurred, this, &Client::connectionError);
    connect(connection, &Connection::disconnected, this, &Client::disconnected);
    connect(connection, &Connection::readyForUse, this, &Client::readyForUse);
}

void Client::readyForUse()
{
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(), connection->peerPort())) return;

    connect(connection,  &Connection::newMessage, this, &Client::newMessage); /////// delete ??????????
    connect(connection, &Connection::newMessage, this, &Client::commandMessageDecoder);
    connect(connection,  &Connection::newByteArray, this, &Client::commandByteArrayDecoder);

    peers.insert(connection->peerAddress(), connection);
    QString nick = connection->name();
    QStringList stringList = nick.split("\n");
    QString usercode = stringList.at(1);
    if (!isPlayerPresent(usercode, connection->peerAddress().toString())){
        Player* player = new Player();
        player->usercode = usercode;
        player->userIP = connection->peerAddress().toString();
        player->username = stringList.at(0);
        player->userstatus = "None";
        playerList.append(player);
        emit updatePlayers();
    }
    if (!nick.isEmpty()) emit newParticipant(nick);
}

void Client::disconnected()
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::connectionError(QAbstractSocket::SocketError /* socketError */)
{
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection *connection)
{
    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
        QString nick = connection->name();
        if (!nick.isEmpty())
            emit participantLeft(nick);
    }
    connection->deleteLater();
}

bool Client::isPlayerPresent(const QString &usercode, const QString &userIP){
    bool isPresent = false;
    for (Player* player: playerList){
        if ((player->usercode == usercode) && (player->userIP == userIP)){
            return true;
        }
    }
    return isPresent;
}


void Client::sendPlayersData(){

    for (Connection *connection : qAsConst(peers)) {
        if (connection->localPort() == server.serverPort()) {
            connection->sendMessage("setPlayerStatus\n" + userstatus);
            connection->sendMessage("setPlayerName\n" + username);
            connection->sendMessage("setRoomName\n" + roomname);
        }
    }
}

void Client::startSendStatus(){
    sendStatusTimer.start();
}

void Client::stopSendStatus(){
    sendStatusTimer.stop();
}

void Client::sendMessageTo(const QString &sendercode, const QString &senderIP, const QString &message){
    for (Connection *connection : qAsConst(peers)) {
        if ((connection->localPort() == server.serverPort()) && (connection->peerAddress().toString() == senderIP) && (connection->name().split("\n").at(1) == sendercode)) {
            connection->sendMessage(message);
            //printf("send message: %s\n", message.toLocal8Bit().constData());
        }
    }
}

void Client::sendByteArrayTo(const QString &sendercode, const QString &senderIP, const QByteArray &array){
    if (array.isEmpty())
        return;

    for (Connection *connection : qAsConst(peers)) {
        if ((connection->localPort() == server.serverPort()) && (connection->peerAddress().toString() == senderIP) && (connection->name().split("\n").at(1) == sendercode)) {
            connection->sendByteArray(array);
        }
    }
}

void Client::commandMessageDecoder(const QString &from, const QString &message){
    QStringList messageList = message.split("\n");
    if (messageList.size() < 2) {
        return;
    }
    QString command = messageList.at(0);
    QString commandPayload = messageList.at(1);
    QStringList fullUsername = from.split("\n");
    if (fullUsername.size() != 3) return;
    QString usercode = fullUsername.at(1);
    QString userIP = fullUsername.at(2);
    //printf("command:%s\n",command.toLocal8Bit().constData());
    if (command == "setPlayerStatus"){
        bool isUpdate = false;
        for (Player* player : playerList){
            if ((player->usercode == usercode) && (player->userIP == userIP) && (player->userstatus != commandPayload)){
                player->userstatus = commandPayload;
                isUpdate = true;
            }
        }
        if (isUpdate){
            emit updatePlayers();
        }
    }

    if (command == "setPlayerName"){
        bool isUpdate = false;
        for (Player* player : playerList){
            if ((player->usercode == usercode) && (player->userIP == userIP) && (player->username != commandPayload)){
                player->username = commandPayload;
                isUpdate = true;
            }
        }
        if (isUpdate){
            emit updatePlayers();
        }
    }

    if (command == "setRoomName"){
        bool isUpdate = false;
        for (Player* player : playerList){
            if ((player->usercode == usercode) && (player->userIP == userIP) && (player->roomUsername != commandPayload)){
                player->roomUsername = commandPayload;
                isUpdate = true;
            }
        }
        if (isUpdate){
            emit updatePlayers();
        }
    }

    if (command == "requestConnectToRoom"){
        emit requestConnectToRoom(from);
    }

    if (command == "permissionConnectToRoom"){
        emit permissionConnectToRoom(from, commandPayload);
    }

    if (command == "disconnectFromRoom"){
        emit disconnectFromRoom(from);
    }

    if (command == "gameMessageResult"){
        int step = messageList.at(2).toInt();
        int id = messageList.at(3).toInt();
        saveMessageToResult(from, commandPayload, step, id);
    }

    if (command == "startGame"){
        emit startGame(from);
    }

    if (command == "endGame"){
        emit endGame(from);
    }

    if (command == "setGameStep") {
        currentGameStep = commandPayload.toInt();
        emit setGameStep(from, commandPayload);
    }

    if (command == "gameShowMessage") {
        emit gameShowMessage(from, commandPayload);
    }

    if (command == "nextGameStep"){
        emit nextGameStep(from);
    }
}

QByteArray Client::commandToByteArray(const QString &command, const QByteArray &array){
    QByteArray arrayResult = command.toUtf8();
    arrayResult = arrayResult.leftJustified(256, ' ');
    arrayResult.append(array);
    return arrayResult;
}

QString Client::byteArrayToCommand(const QByteArray &array){
    return QString::fromUtf8(array).trimmed();
}

void Client::commandByteArrayDecoder(const QString &from, const QByteArray &array){
    QStringList fullUsername = from.split("\n");
    if (fullUsername.size() != 3) return;
    QString usercode = fullUsername.at(1);
    QString userIP = fullUsername.at(2);
    QStringList messageList = byteArrayToCommand(array.left(256)).split('\n');
    //printf("command %s\n", (byteArrayToCommand(array.left(256)) + "#").toLocal8Bit().constData());
    QByteArray arrayPayload = array.right(array.size() - 256);

    QString command = messageList.at(0);

    if (command == "gameImageResult"){
        int step = messageList.at(1).toInt();
        int id = messageList.at(2).toInt();
        emit saveImageToResult(from, arrayPayload, step, id);
    }

    if (command == "gameShowImage") {
        emit gameShowImage(from, arrayPayload);
    }

}

void Client::saveImageToResult(const QString &from, const QByteArray &array, const int &step, const int &id){
        ResultRecord* record = new ResultRecord;
        QImage image;
        image.loadFromData(array, "PNG");
        record->image = image;
        record->message = "";
        record->playerID = id;
        record->gameStep = step;
        result.append(record);
}

void Client::saveMessageToResult(const QString &from, const QString &message, const int &step, const int &id){
        ResultRecord* record = new ResultRecord;
        record->message = message;
        record->playerID = id;
        record->gameStep = step;
        result.append(record);
}

ResultRecord* Client::getFromResult(int step, int id){
    ResultRecord* resultrecord = NULL;
    for (ResultRecord* record : result){
        if ((record->gameStep == step) && (record->playerID == id)){
            resultrecord = record;
        }
    }
    return resultrecord;
}
