#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintwindow.h>
#include <client.h>
#include <createroomwindow.h>
#include <connectwindow.h>
#include <resultwindow.h>
#include <aboutwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private:
    Ui::MenuWindow *ui;
    PaintWindow* paintWindow;
    CreateRoomWindow* createRoomWindow;
    ConnectWindow* connectWindow;
    ResultWindow* resultWindow;
    AboutWindow* aboutWindow;
    Client* client;
    QString username;
    QString usercode;
    void initUI();

private slots:
    void btnCreateRoomClick();
    void btnConnectClick();
    void btnAboutClick();
    void btnExitClick();
    void btnResultClick(); // delete
    void btnPaintClick(); // delete
    void changeUserName();
    void onStartGame();
    void onEndGame();
};
#endif // MAINWINDOW_H
