#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QWidget>
#include <client.h>
#include "roomrecord.h"

QT_BEGIN_NAMESPACE
namespace Ui {class ConnectWindow; }
QT_END_NAMESPACE

class ConnectWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectWindow(QWidget *parent = nullptr, Client *client = nullptr);
    ~ConnectWindow();

private:
    Ui::ConnectWindow *ui;
    Client *client;
    QString roomName;
    QString roomCode;
    QString roomIP;

    QString waitFromRoomCode;
    QString waitFromRoomIP;

    RoomRecord* currentRoomRecord = NULL;
    QList<RoomRecord*> rooms;
    bool isWaitRequest;
    QTimer requestTimer;

    void showUsername();
    void clearRooms();
    QString getRoomName(const QString &code, const QString &IP);


public slots:
    void updatePlayerList();

private slots:
    void btnExitClick();
    void btnConnectClick();
    void btnDisonnectClick();
    void onSelectRoom(RoomRecord* record);
    void requestTimerTimeout();
    void onPermissionReceived(const QString &from, const QString &id);

signals:
    void toMenuWindow();


};

#endif // CONNECTWINDOW_H
