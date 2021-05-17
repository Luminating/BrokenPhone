#include "createroomwindow.h"
#include "ui_createroomwindow.h"
#include <QStringListModel>

CreateRoomWindow::CreateRoomWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::CreateRoomWindow)
{
    ui->setupUi(this);
    initUi();
    this->client = client;
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
    connect(client, SIGNAL(updatePlayers()), this, SLOT(updatePlayerList()));
    connect(client, SIGNAL(requestConnectToRoom(QString)), this, SLOT(permissionConnectToRoom(QString)));
    connect(ui->btnIncCount, SIGNAL(clicked()), this, SLOT(btnIncCountClick()));
    connect(ui->btnDecCount, SIGNAL(clicked()), this, SLOT(btnDecCountClick()));
    connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(btnPlayClick()));
    //connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
    //connect(ui->btnAbout, SIGNAL(clicked()), this, SLOT(btnAboutClick()));

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
}


CreateRoomWindow::~CreateRoomWindow()
{
    delete ui;
}


void CreateRoomWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

void CreateRoomWindow::updatePlayerList(){
    /*
    showUsername();
    QStringList playersList;
    for (Player* player: client->playerList){
        playersList.append(player->username + "|" + player->usercode + "|" + player->userIP + "|" + player->userstatus + "|" + player->roomUsername);
    }
    QStringListModel* stringListModel = new QStringListModel;
    stringListModel->setStringList(playersList);
    ui->listPlayers->setModel(stringListModel);
    */
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
    showLight(true);
    client->userstatus = "Room0/" + QString::number(client->maxPlayerCount);
    client->playersInRoom.clear();
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

void CreateRoomWindow::sendPlayersInRoom(){
 //   for (Player* playerTo : client->playersInRoom){
 //       QString message = "sendPlayersInRoom";
 //
 //
 //
 //   }

}

void CreateRoomWindow::btnPlayClick(){


}
