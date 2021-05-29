#include "connectwindow.h"
#include "ui_connectwindow.h"
#include <QStringListModel>
#include <QFontDatabase>
#include <QTime>

static const int WaitRequestInterval = 5 * 1000; // 5 sec

ConnectWindow::ConnectWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    this->client = client;
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(btnConnectClick()));
    connect(ui->btnDisconnect, SIGNAL(clicked()), this, SLOT(btnDisconnectClick()));
    connect(client, SIGNAL(updatePlayers()), this, SLOT(updatePlayerList()));
    connect(&requestTimer, SIGNAL(timeout()), this, SLOT(requestTimerTimeout()));
    connect(client, SIGNAL(permissionConnectToRoom(QString, QString)), this, SLOT(onPermissionReceived(QString, QString)));
    connect(client, SIGNAL(deleteRoom()), this, SLOT(onDeleteRoom()));
    connect(client, SIGNAL(startGame()), this, SLOT(onStartGame()));
    showUsername();
    selectedRecordName = "";
    selectedRecordCode = "";
    selectedRecordIP = "";
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
    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");

    int id = QFontDatabase::addApplicationFont(":fonts/20322");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont labelFont(family);
    labelFont.setPointSize(24);
    ui->labelMessage->setFont(labelFont);
    labelFont.setPointSize(20);
    ui->labelConnect->setFont(labelFont);
    ui->labelDisconnect->setFont(labelFont);
    ui->labelExit->setFont(labelFont);

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
            if ((roomrecord->roomCode == selectedRecordCode) && (roomrecord->roomIP == selectedRecordIP)) {
                roomrecord->fillBackground("color: rgb(200, 0, 0); font-size: 22pt;");
            } else {
                roomrecord->fillBackground("color: rgb(0, 0, 0); font-size: 22pt;");
            }

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
    if (selectedRecordCode == "") return; // if not any record selected in list
    if (isWaitRequest) return;
    if (client->roomname != "") return; //  if room present
    QString message = "requestConnectToRoom\n\n";
    waitFromRoomCode = selectedRecordCode;
    waitFromRoomIP = selectedRecordIP;
    client->sendMessageTo(waitFromRoomCode, waitFromRoomIP, message);
    isWaitRequest = true;
    requestTimer.start();
}

void ConnectWindow::btnDisconnectClick(){
    if (client->roomname == "") return;
    isWaitRequest = false;
    requestTimer.stop();
    QString roomCode = client->roomname.split("\n").at(1);
    QString roomIP = client->roomname.split("\n").at(2);
    QString message = "disconnectFromRoom\n\n";
    client->sendMessageTo(roomCode, roomIP, message);
    client->roomname = "";
    client->userId = -1;
    selectedRecordName = "";
    selectedRecordCode = "";
    selectedRecordIP = "";
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
    ui->labUserName->setText("Превышено время ожидания");
}

void ConnectWindow::delay(int sec){
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void ConnectWindow::onPermissionReceived(const QString &from, const QString &id){
    if (!isWaitRequest) return;
    if ((waitFromRoomCode == from.split("\n").at(1)) && (waitFromRoomIP == from.split("\n").at(2))){
        requestTimer.stop();
        isWaitRequest = false;
        QString roomCode = from.split("\n").at(1);
        QString roomIP = from.split("\n").at(2);
        QString roomName = getRoomName(roomCode, roomIP);
        client->roomname = roomName + "\n" + roomCode + "\n" + roomIP;
        client->userId = id.toInt();
        ui->labelMessage->setText("Вы подключены к комнате " + roomName);
        showLight(true);
    }
}

void ConnectWindow::onDeleteRoom(){
    client->roomname = "";
    client->userId = -1;
    selectedRecordName = "";
    selectedRecordCode = "";
    selectedRecordIP = "";
    waitFromRoomCode ="";
    waitFromRoomIP = "";
    ui->labelMessage->setText("Комната удалена");
    delay(3);
    ui->labelMessage->setText("Вы не подключены к комнате");
    showLight(false);
    updatePlayerList();
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
    selectedRecordName = record->roomName;
    selectedRecordCode = record->roomCode;
    selectedRecordIP = record->roomIP;
    for (int i = 0; i < rooms.size(); i++){
        if ((rooms.at(i)->roomCode == selectedRecordCode) && (rooms.at(i)->roomIP == selectedRecordIP)) {
            rooms.at(i)->fillBackground("color: rgb(200, 0, 0); font-size: 22pt;");
        } else {
            rooms.at(i)->fillBackground("color: rgb(0, 0, 0); font-size: 22pt;");
        }
    }
}

void ConnectWindow::onStartGame() {
    requestTimer.stop();   /// ?????
    isWaitRequest = false; /// ???
    this->close();
}

void ConnectWindow::btnExitClick(){
    if (client->roomname == ""){
        this->close();
        emit toMenuWindow();
    } else {
        QString temp = ui->labelMessage->text();
        ui->labelMessage->setText("Для выхода - отключитесь от комнаты");
        delay(2);
        ui->labelMessage->setText(temp);
    }
}
