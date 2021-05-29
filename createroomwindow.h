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
    QTimer stepTimer;
    int stepSecondsLeft;
    bool isGameOver;
    void initUi();
    void showIndicator(int number);
    void showLight(bool isOn);
    QString getPlayerName(const QString &code, const QString &IP);    
    void stepBody(bool isError);
    int getNextId(int currentId);
    void endGame();
    void delay(int sec);


public slots:
    void updatePlayerList();

private slots:
    void btnExitClick();
    void btnCreateRoomClick();
    void btnDeleteRoomClick();
    void permissionConnectToRoom(const QString &username);
    void disconnectFromRoom(const QString &username);
    void btnIncCountClick();
    void btnDecCountClick();
    void btnPlayClick();
    void nextGameStep();

signals:
    void toMenuWindow();

};

#endif // CREATEROOMWINDOW_H
