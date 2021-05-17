#include "menuwindow.h"
#include "ui_menuwindow.h"
#include <QRandomGenerator>

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    username = "player " + QString::number(QRandomGenerator::global()->bounded(99));
    usercode = "\n" + QString::number(QRandomGenerator::global()->bounded(99999));
    ui->editUsername->setText(username);
    this->showFullScreen();
    client = new Client(username + usercode);
    client->userstatus = "None";
    client->startSendStatus();
    paintWindow = new PaintWindow(nullptr, client);
    createRoomWindow = new CreateRoomWindow(nullptr, client);
    connectWindow = new ConnectWindow(nullptr, client);
    resultWindow = new ResultWindow(nullptr, client);

    connect(paintWindow, &PaintWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(createRoomWindow, &CreateRoomWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(connectWindow, &ConnectWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(resultWindow, &ResultWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(btnConnectClick()));
    connect(ui->btnAbout, SIGNAL(clicked()), this, SLOT(btnAboutClick()));
    connect(ui->btnResult, SIGNAL(clicked()), this, SLOT(btnResultClick()));  ////  delete
    connect(ui->btnPaint, SIGNAL(clicked()), this, SLOT(btnPaintClick()));  ////  delete
    connect(ui->editUsername, SIGNAL(returnPressed()), this, SLOT(changeUserName()));
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::btnConnectClick(){
        connectWindow->showFullScreen();
        changeUserName();
        connectWindow->updatePlayerList();
        this->close();
}

void MenuWindow::btnCreateRoomClick(){
        createRoomWindow->showFullScreen();
        changeUserName();
        createRoomWindow->updatePlayerList();
        this->close();
}

void MenuWindow::btnAboutClick(){
        //aboutWindow->showFullScreen();
        //this->close();
}

void MenuWindow::changeUserName(){
    username = ui->editUsername->text();
    client->username = username + usercode;
}

void MenuWindow::btnResultClick(){  //// delete
        resultWindow->showFullScreen();
        this->close();
}

void MenuWindow::btnPaintClick(){  //// delete
        paintWindow->showFullScreen();
        this->close();
}
