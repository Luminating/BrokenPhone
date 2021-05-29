#include "createroomwindow.h"
#include "ui_createroomwindow.h"
#include <QStringListModel>
#include <QBuffer>
#include <QMutableListIterator>
#include <QFontDatabase>
#include <QTime>

static const int MaxGameStepInterval = 1 * 60 + 10;  // in sec

CreateRoomWindow::CreateRoomWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::CreateRoomWindow)
{
    ui->setupUi(this);
    initUi();
    this->client = client;
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
    connect(ui->btnDeleteRoom, SIGNAL(clicked()), this, SLOT(btnDeleteRoomClick()));
    connect(client, SIGNAL(updatePlayers()), this, SLOT(updatePlayerList()));
    connect(client, SIGNAL(requestConnectToRoom(QString)), this, SLOT(permissionConnectToRoom(QString)));
    connect(client, SIGNAL(disconnectFromRoom(QString)), this, SLOT(disconnectFromRoom(QString)));
    connect(ui->btnIncCount, SIGNAL(clicked()), this, SLOT(btnIncCountClick()));
    connect(ui->btnDecCount, SIGNAL(clicked()), this, SLOT(btnDecCountClick()));
    connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(btnPlayClick()));
    connect(&stepTimer, SIGNAL(timeout()), this, SLOT(nextGameStep())); ///////////


    stepSecondsLeft = MaxGameStepInterval;
    stepTimer.setInterval(1 * 1000); // 1 sec

    ui->labelMessage->setText("Комната " + client->username.split("\n").at(0));
    showIndicator(client->maxPlayerCount);
    showLight(false);

}

void CreateRoomWindow::initUi(){
    ui->btnIncCount->setStyleSheet("QPushButton:pressed {background: url(:btnRightActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRightInactive);}");
    ui->btnDecCount->setStyleSheet("QPushButton:pressed {background: url(:btnLeftActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnLeftInactive);}");
    ui->btnCreateRoom->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnDeleteRoom->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");
    ui->btnPlay->setStyleSheet("QPushButton:pressed {background: url(:btnPlayActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnPlayInactive);}");
    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");

    int id = QFontDatabase::addApplicationFont(":fonts/20322");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont labelFont(family);
    labelFont.setPointSize(26);
    ui->listPlayers->setFont(labelFont);
    ui->labelMessage->setFont(labelFont);
    labelFont.setPointSize(20);
    ui->labelBtn1->setFont(labelFont);
    ui->labelBtn2->setFont(labelFont);
    ui->labelExit->setFont(labelFont);
    labelFont.setPointSize(18);
    ui->labelRoomCount->setFont(labelFont);

}


CreateRoomWindow::~CreateRoomWindow()
{
    delete ui;
}

void CreateRoomWindow::updatePlayerList(){
    ui->labelMessage->setText("Комната " + client->username.split("\n").at(0));
    QStringList playersList;
    for (Player* player: client->playersInRoom){
        playersList.append(QString::number(player->id + 1) + ".  " + getPlayerName(player->usercode, player->userIP));
    }
    QStringListModel* stringListModel = new QStringListModel;
    stringListModel->setStringList(playersList);
    ui->listPlayers->setModel(stringListModel);
}

QString CreateRoomWindow::getPlayerName(const QString &code, const QString &IP){
    QString name = "not found";
    for (int i = 0; i < client->playerList.size(); i++){
        if ((client->playerList.at(i)->usercode == code) && (client->playerList.at(i)->userIP == IP)){
            name = client->playerList.at(i)->username;
        }
    }
    return name;
}

void CreateRoomWindow::permissionConnectToRoom(const QString &username){
    int id = getNextPlayerId();
    if (id < client->maxPlayerCount) {        
        Player* player = new Player();
        player->id = id;
        player->username = username.split("\n").at(0);
        player->usercode = username.split("\n").at(1);
        player->userIP = username.split("\n").at(2);
        client->playersInRoom.append(player);
        client->userstatus = "Room" + QString::number(client->playersInRoom.size()) +"/" + QString::number(client->maxPlayerCount);
        QString message = "permissionConnectToRoom\n" + QString::number(id);
        client->sendMessageTo(username.split("\n").at(1), username.split("\n").at(2), message);
        updatePlayerList();
    }
}

void CreateRoomWindow::disconnectFromRoom(const QString &username){
    QString usercode = username.split("\n").at(1);
    QString userIP = username.split("\n").at(2);
    QMutableListIterator<Player*> player(client->playersInRoom);
    while (player.hasNext()) {
        Player* currPlayer = player.next();
        if ((currPlayer->usercode == usercode) && (currPlayer->userIP == userIP)){
            player.remove();
        }
    }
    client->userstatus = "Room" + QString::number(client->playersInRoom.size()) +"/" + QString::number(client->maxPlayerCount);
    updatePlayerList();

}

int CreateRoomWindow::getNextPlayerId(){
    int id = 0;
    bool isIdPresent = false;
    for (int i = 0; i < client->maxPlayerCount + 1; i++){
        for(Player* player : client->playersInRoom) {
            if (id == player->id) {
                isIdPresent = true;
            }
        }
        if (isIdPresent) {
            id++;
            isIdPresent = false;
        } else {
            break;
        }
    }
    return id;
}

void CreateRoomWindow::btnCreateRoomClick(){
    if (client->userstatus.left(4) == "Room") return;    
    client->userstatus = "Room0/" + QString::number(client->maxPlayerCount);
    client->playersInRoom.clear();
    showLight(true);
}

void CreateRoomWindow::btnDeleteRoomClick(){
    if (client->userstatus.left(4) != "Room") return;
    if (client->isPlayGame) return;
    for(Player* player : client->playersInRoom){
        QString message = "deleteRoom\n\n";
        client->sendMessageTo(player->usercode, player->userIP, message);
    }

    client->userstatus = "None";
    client->playersInRoom.clear();
    showLight(false);
}


void CreateRoomWindow::btnIncCountClick(){
    if (client->maxPlayerCount < 9) {
        client->maxPlayerCount++;
        showIndicator(client->maxPlayerCount);
    }
}

void CreateRoomWindow::btnDecCountClick(){
    if (client->maxPlayerCount > 3) {
        client->maxPlayerCount--;
        showIndicator(client->maxPlayerCount);
    }

}

void CreateRoomWindow::showIndicator(int number){
    if ((number < 10) && (number > -1)) {
        ui->labIndicator_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(number) + ");}");
    }
}

void CreateRoomWindow::showLight(bool isOn){
    if (isOn){
       ui->labelLight->setStyleSheet("QLabel{background: url(:lightON);}");
    } else {
        ui->labelLight->setStyleSheet("QLabel{background: url(:lightOFF);}");
    }
}

void CreateRoomWindow::delay(int sec){
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void CreateRoomWindow::nextGameStep(){
    stepSecondsLeft--;

    int returnResultPlayersCount = 0;
    for (ResultRecord* record : client->result){
        if (record->gameStep == client->currentGameStep){
            returnResultPlayersCount++;
        }
    }
    bool isError = false;
    stepTimer.setInterval(1 * 1000);
    if ((stepSecondsLeft < 0) || (returnResultPlayersCount == client->maxPlayerCount)) {
       // printf("step =%d stepSecondsLeft =%d returnResultPlayersCount =%d\n", client->currentGameStep, stepSecondsLeft, returnResultPlayersCount);
        stepTimer.stop();
        if (returnResultPlayersCount != client->maxPlayerCount){
            //printf("stepBody Error step %d\n", client->currentGameStep);

            isError = true;
        }
        stepBody(isError);
    } else {
        stepTimer.start();
    }
}

void CreateRoomWindow::stepBody(bool isError){
    if (isError){
        ui->labelMessage->setText("Ошибка: отключился игрок");
        for(Player* player : client->playersInRoom){
            QString message = "gameStopError\nИграть дальше нельзя, отключился игрок";
            client->sendMessageTo(player->usercode, player->userIP, message);
        }
        client->isPlayGame = false;
        return;
    }

    if (client->currentGameStep == client->maxPlayerCount - 1) {
        endGame();
        return;
    }

    for(Player* player : client->playersInRoom){
        int nextId = getNextId(player->id);

        if ((client->currentGameStep % 2) == 0) { /// step 0,2,4,.. - text , step 1,3,5, - image
            QString message = "gameShowMessage\n" + client->getFromResult(client->currentGameStep, nextId)->message;
            client->sendMessageTo(player->usercode, player->userIP, message);
        } else {
            QImage image = client->getFromResult(client->currentGameStep, nextId)->image;
            QByteArray array;
            QBuffer buffer(&array);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");

            array = client->commandToByteArray("gameShowImage\n*", array);
            client->sendByteArrayTo(player->usercode, player->userIP, array);
        }
    }

    client->currentGameStep++;
    for(Player* player : client->playersInRoom){
        QString message = "setGameStep\n" + QString::number(client->currentGameStep);
        client->sendMessageTo(player->usercode, player->userIP, message);
    }

    for(Player* player : client->playersInRoom){
        QString message = "nextGameStep\n\n";
        client->sendMessageTo(player->usercode, player->userIP, message);
    }
    if(!isGameOver){
        stepSecondsLeft = MaxGameStepInterval;
        nextGameStep();
    }
}

void CreateRoomWindow::endGame(){
    isGameOver = true;

   // printf("Game Over\n");
    for(Player* player : client->playersInRoom){  /// send to all results
        for(ResultRecord* record : client->result){
            if (!record->image.isNull()) { //// image
                QByteArray array;
                QBuffer buffer(&array);
                buffer.open(QIODevice::WriteOnly);
                record->image.save(&buffer, "PNG");
                array = client->commandToByteArray("gameImageResult\n" + QString::number(record->gameStep) + "\n" + QString::number(record->playerID), array);
                client->sendByteArrayTo(player->usercode, player->userIP, array);

            } else {  //// message
                QString message = "gameMessageResult\n" + record->message + "\n" + QString::number(record->gameStep) + "\n" + QString::number(record->playerID);
                client->sendMessageTo(player->usercode, player->userIP, message);
            }
        }
    }

    for(Player* player : client->playersInRoom){
        QString message = "endGame\n\n";
        client->sendMessageTo(player->usercode, player->userIP, message);
    }
    client->isPlayGame = false;
    //// end, goto ??? Window


}

int CreateRoomWindow::getNextId(int currentId){
    int result = currentId;
    if (currentId == client->maxPlayerCount - 1){
        result = 0;
    } else {
        result++;
    }
    return result;
}

void CreateRoomWindow::btnPlayClick(){
    if (client->maxPlayerCount != client->playersInRoom.size()){
        QString temp = ui->labelMessage->text();
        ui->labelMessage->setText("Недостаточно подключено игроков");
        delay(2);
        ui->labelMessage->setText(temp);
        return;
    }
    if (client->isPlayGame) return;
    client->isPlayGame = true;
    for(Player* player : client->playersInRoom) {
        QString message = "gameShowMessage\nНачинаем игру - введите фразу и нажмите ГОТОВО";
        client->sendMessageTo(player->usercode, player->userIP, message);
        message = "startGame\n\n";
        client->sendMessageTo(player->usercode, player->userIP, message);


        QImage image = QImage(":/startImage");
        //QDir::currentPath() + "/images_menu/" + imgName
        QByteArray array;
        QBuffer buffer(&array);
        buffer.open(QIODevice::WriteOnly);
        image.save(&buffer, "PNG");

        array = client->commandToByteArray("gameShowImage\n*", array);
        client->sendByteArrayTo(player->usercode, player->userIP, array);

        isGameOver = false;
        stepSecondsLeft = MaxGameStepInterval;
        nextGameStep();
    }
}

void CreateRoomWindow::btnExitClick(){
    if (client->userstatus.left(4) != "Room"){
        this->close();
        emit toMenuWindow();
    } else {
        QString temp = ui->labelMessage->text();
        ui->labelMessage->setText("Для выхода - удалите комнату");
        delay(2);
        ui->labelMessage->setText(temp);
    }
}
