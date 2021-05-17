#include "resultwindow.h"
#include "ui_resultwindow.h"

ResultWindow::ResultWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::ResultWindow)
{
    ui->setupUi(this);
    this->client = client;
    initUi();
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnIncCount, SIGNAL(clicked()), this, SLOT(btnIncCountClick()));
    connect(ui->btnDecCount, SIGNAL(clicked()), this, SLOT(btnDecCountClick()));
    connect(ui->btnUpdate, SIGNAL(clicked()), this, SLOT(updateResultList()));
    showIndicator(fork);
}

void ResultWindow::initUi(){
    ui->btnIncCount->setStyleSheet("QPushButton:pressed {background: url(:btnRightActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRightInactive);}");
    ui->btnDecCount->setStyleSheet("QPushButton:pressed {background: url(:btnLeftActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnLeftInactive);}");
}

ResultWindow::~ResultWindow()
{
    delete ui;
}

void ResultWindow::btnIncCountClick(){
    if (fork < 9) {
        fork++;
        showIndicator(fork);
    }
}

void ResultWindow::btnDecCountClick(){
    if (fork > 0) {
        fork--;
        showIndicator(fork);
    }

}

void ResultWindow::showIndicator(int number){
    if ((number < 10) && (number > -1)) {
        ui->labIndicator_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(number) + ");}");
    }
}

void ResultWindow::updateResultList(){
    //for (int step = 0; step < 10; step++){  //// todo
        for (ResultRecord* record : client->result) {
            if (record->playerID == fork) {
                if (!record->image.isNull()) {


                }
            }
        }

    //}

}


void ResultWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

