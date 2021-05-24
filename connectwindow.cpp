#include "connectwindow.h"
#include "ui_connectwindow.h"
#include <QStringListModel>
#include <QFontDatabase>

static const int WaitRequestInterval = 5 * 1000;

ConnectWindow::ConnectWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    this->client = client;
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(btnConnectClick()));
    connect(ui->btnDisconnect, SIGNAL(clicked()), this, SLOT(btnDisonnectClick()));
    connect(client, SIGNAL(updatePlayers()), this, SLOT(updatePlayerList()));
    connect(&requestTimer, SIGNAL(timeout()), this, SLOT(requestTimerTimeout()));
    connect(client, SIGNAL(permissionConnectToRoom(QString, QString)), this, SLOT(onPermissionReceived(QString, QString)));
    connect(client, SIGNAL(startGame(QString)), this, SLOT(onStartGame(QString)));
    showUsername();
    roomName = "";
    roomCode = "";
    roomIP = "";
    waitFromRoomCode ="";
    waitFromRoomIP = "";
    isWaitRequest = false;
    requestTimer.setInterval(WaitRequestInterval);
    initUi();
}

void ConnectWindow::initUi(){
    //ui->labelMessage->setText("You are not connected to the room");
    ui->labelMessage->setText("Вы не подключены к комнате");
    showLight(false);
    ui->btnConnect->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnDisconnect->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");
    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:powerOFF);}"
                                           "QPushButton {border: none;"
                                           "background: url(:powerON);}");

    int id = QFontDatabase::addApplicationFont(":fonts/20322");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont menuFont(family);
    menuFont.setPointSize(24);
    ui->labelMessage->setFont(menuFont);
    menuFont.setPointSize(20);
    ui->labelConnect->setFont(menuFont);
    ui->labelDisconnect->setFont(menuFont);

}


ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::showUsername(){
    ui->labUserName->setText(client->username.split("\n").at(0));
}

void ConnectWindow::showLight(bool isOn){
    if (isOn){
       ui->labelLight->setStyleSheet("QLabel{background: url(:lightON);}");
    } else {
        ui->labelLight->setStyleSheet("QLabel{background: url(:lightOFF);}");
    }
}


void ConnectWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

void ConnectWindow::updatePlayerList(){
    showUsername();

    clearRooms();
    int count = 0;
    for (Player* player: client->playerList){
        if (player->userstatus.left(4) == "Room") {
            int playersCount = player->userstatus.mid(4,1).toInt();
            int maxPlayers = player->userstatus.mid(6,1).toInt();;
            RoomRecord* roomrecord = new RoomRecord(this);
            roomrecord = roomrecord->init(roomrecord, player->username, player->usercode, player->userIP, maxPlayers, playersCount);
            connect(roomrecord, SIGNAL(selectRecord(RoomRecord*)), this, SLOT(onSelectRoom(RoomRecord*)));
            ui->roomsVbox->addWidget(roomrecord);
            ui->roomsAreaWidget->resize(ui->roomsAreaWidget->geometry().width(), ui->roomsAreaWidget->geometry().height() + 60);
            rooms.append(roomrecord);
            count++;
        }
    }
}

void ConnectWindow::clearRooms(){
    rooms.clear();
    int count = ui->roomsVbox->count();
    for (int i = 0; i < count; i++){
        QWidget* widget = ui->roomsVbox->itemAt(0)->widget();
        ui->roomsVbox->removeWidget(widget);
        widget->setParent(NULL);
        widget->deleteLater();
    }
    ui->roomsAreaWidget->resize(ui->roomsAreaWidget->geometry().width(), 0);
}



void ConnectWindow::btnConnectClick(){
    if (currentRoomRecord == NULL) return;
    if (isWaitRequest) return;
    if (roomCode != "") return; //  ----------------------- to chesk client.
    QString message = "requestConnectToRoom\n\n";
    waitFromRoomCode = currentRoomRecord->roomCode;
    waitFromRoomIP = currentRoomRecord->roomIP;
    client->sendMessageTo(waitFromRoomCode, waitFromRoomIP, message);
    isWaitRequest = true;
    requestTimer.start();

}

void ConnectWindow::btnDisonnectClick(){
    if (isWaitRequest) return;
    if (roomCode == "") return;
    QString message = "disconnectFromRoom\n\n";
    client->sendMessageTo(roomCode, roomIP, message);
    roomCode = "";
    roomIP = "";
    roomName = "";
    waitFromRoomCode ="";
    waitFromRoomIP = "";
    //ui->labelMessage->setText("You are not connected to the room");
    ui->labelMessage->setText("Вы не подключены к комнате");
    showLight(false);
}


void ConnectWindow::requestTimerTimeout(){
    waitFromRoomCode ="";
    waitFromRoomIP = "";
    isWaitRequest = false;
    ui->labUserName->setText("timer timeout !!!");
}


void ConnectWindow::onPermissionReceived(const QString &from, const QString &id){ ///  todo player id
    if (!isWaitRequest) return;
    if ((waitFromRoomCode == from.split("\n").at(1)) && (waitFromRoomIP == from.split("\n").at(2))){
        requestTimer.stop();
        isWaitRequest = false;
        roomCode = from.split("\n").at(1);
        roomIP = from.split("\n").at(2);
        roomName = getRoomName(roomCode, roomIP);
        client->roomname = roomName + "\n" + roomCode + "\n" + roomIP;
        client->userId = id.toInt();
        ui->labelMessage->setText("Вы подключены к комнате " + roomName);
        showLight(true);
    }
}

QString ConnectWindow::getRoomName(const QString &code, const QString &IP){
    QString name = "not found";
    for (int i = 0; i < rooms.size(); i++){
        if ((rooms.at(i)->roomCode == code) && (rooms.at(i)->roomIP == IP)){
            name = rooms.at(i)->roomName;
        }
    }
    return name;
}

void ConnectWindow::onSelectRoom(RoomRecord* record){
    currentRoomRecord = record;
   // printf("selecterd record %d", currentRoomRecord->id);

    for (int i = 0; i < rooms.size(); i++){
        if (rooms.at(i) == currentRoomRecord) {
            rooms.at(i)->fillBackground("background-color: rgba(0, 0, 0, 0.2); color: rgb(200, 0, 0); font-size: 22pt;");
        } else {
            rooms.at(i)->fillBackground("background-color: rgba(150, 150, 150, 0.2); color: rgb(0, 0, 0); font-size: 22pt;");
        }
    }
}

void ConnectWindow::onStartGame(const QString &from) {
    requestTimer.stop();   /// ?????
    isWaitRequest = false; /// ???
    this->close();
}
