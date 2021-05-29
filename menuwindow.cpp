#include "menuwindow.h"
#include "ui_menuwindow.h"
#include <QRandomGenerator>
#include <QFontDatabase>

MenuWindow::MenuWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MenuWindow)
{
    ui->setupUi(this);
    initUI();
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
    aboutWindow = new AboutWindow(nullptr);

    connect(paintWindow, &PaintWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(createRoomWindow, &CreateRoomWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(connectWindow, &ConnectWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(resultWindow, &ResultWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(aboutWindow, &AboutWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
    connect(ui->btnConnect, SIGNAL(clicked()), this, SLOT(btnConnectClick()));
    connect(ui->btnAbout, SIGNAL(clicked()), this, SLOT(btnAboutClick()));
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnResult, SIGNAL(clicked()), this, SLOT(btnResultClick()));  ////  delete
    connect(ui->btnPaint, SIGNAL(clicked()), this, SLOT(btnPaintClick()));  ////  delete
    connect(ui->editUsername, SIGNAL(returnPressed()), this, SLOT(changeUserName()));
    connect(client, SIGNAL(startGame()), this, SLOT(onStartGame()));
    connect(client, SIGNAL(endGame()), this, SLOT(onEndGame()));
}


void MenuWindow::initUI(){
    ui->btnConnect->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnCreateRoom->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnAbout->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");

    int id = QFontDatabase::addApplicationFont(":fonts/19287");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont titleFont(family);
    titleFont.setPointSize(120);
    ui->labGameName->setFont(titleFont);

    id = QFontDatabase::addApplicationFont(":fonts/20322");
    family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont menuFont(family);
    menuFont.setPointSize(26);
    ui->labelConnect->setFont(menuFont);
    ui->labelCreate->setFont(menuFont);
    ui->labelAbout->setFont(menuFont);
    ui->labelExit->setFont(menuFont);
    ui->labelName->setFont(menuFont);
    ui->editUsername->setFont(menuFont);
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
        aboutWindow->showFullScreen();
        this->close();
}

void MenuWindow::btnExitClick(){
    exit(0);
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

void MenuWindow::onStartGame(){
    paintWindow->showFullScreen();
}

void MenuWindow::onEndGame(){
    resultWindow->showFullScreen();
}
