#include "roomrecord.h"
#include "ui_roomrecord.h"

RoomRecord::RoomRecord(QWidget *parent) : QWidget(parent), ui(new Ui::RoomRecord)
{
    ui->setupUi(this);
    connect(ui->btnRecord, SIGNAL(clicked()), this, SLOT(btnRecordClick()));
}

RoomRecord* RoomRecord::init(RoomRecord* record, QString initRoomName, QString initRoomCode, QString initRoomIp, int initMaxPlayers, int initplayersCount){
    record->roomName = initRoomName;
    ui->labName->setText(initRoomName);
    record->roomCode = initRoomCode;
    record->roomIP = initRoomIp;
    record->maxPlayers = initMaxPlayers;
    ui->labMaxPlayers->setText(QString::number(initMaxPlayers));
    record->playersCount = initplayersCount;
    ui->labPlayersCount->setText(QString::number(initplayersCount));
    return record;
}

RoomRecord::~RoomRecord()
{
    delete ui;
}

void RoomRecord::btnRecordClick(){
    emit selectRecord(this);
}

void RoomRecord::fillBackground(QString style){
    setStyleSheet(style);
}
