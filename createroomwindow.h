#ifndef CREATEROOMWINDOW_H
#define CREATEROOMWINDOW_H
#include <QWidget>
#include <client.h>

QT_BEGIN_NAMESPACE
namespace Ui {class CreateRoomWindow; }
QT_END_NAMESPACE

class CreateRoomWindow : public QWidget
{
    Q_OBJECT
public:
    explicit CreateRoomWindow(QWidget *parent = nullptr, Client *client = nullptr);
    ~CreateRoomWindow();


private:
    Ui::CreateRoomWindow *ui;
    Client *client;
    int getNextPlayerId();
    void initUi();
    void showIndicator(int number);
    void showLight(bool isOn);
    QString getPlayerName(const QString &code, const QString &IP);
    void sendPlayersInRoom();

public slots:
    void updatePlayerList();

private slots:
    void btnExitClick();
    void btnCreateRoomClick();
    void permissionConnectToRoom(const QString &username);
    void btnIncCountClick();
    void btnDecCountClick();
    void btnPlayClick();

signals:
    void toMenuWindow();

};

#endif // CREATEROOMWINDOW_H
