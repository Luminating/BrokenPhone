#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintwindow.h>
#include <client.h>
#include <createroomwindow.h>
#include <connectwindow.h>
#include <resultwindow.h>

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
    Client* client;
    QString username;
    QString usercode;

private slots:
    void btnCreateRoomClick();
    void btnConnectClick();
    void btnAboutClick();
    void btnResultClick(); // delete
    void btnPaintClick(); // delete
    void changeUserName();

};
#endif // MAINWINDOW_H
