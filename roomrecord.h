#ifndef ROOMRECORD_H
#define ROOMRECORD_H

#include <QWidget>

namespace Ui {class RoomRecord;}

class RoomRecord : public QWidget
{
    Q_OBJECT

public:
    explicit RoomRecord(QWidget *parent = nullptr);
    ~RoomRecord();

    QString roomName;
    QString roomCode;
    QString roomIP;
    int maxPlayers = 3;
    int playersCount = 0;

    RoomRecord* init(RoomRecord* record, QString initRoomName, QString initRoomCode, QString initRoomIp, int initMaxPlayers, int initplayersCount);
    void fillBackground(QString style);

private:
    Ui::RoomRecord *ui;

private slots:
    void btnRecordClick();

signals:
    void selectRecord(RoomRecord* record);

};

#endif // ROOMRECORD_H
