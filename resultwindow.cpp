#include "resultwindow.h"
#include "ui_resultwindow.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QFontDatabase>
#include <QTime>

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
    ui->btnUpdate->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");
    int id = QFontDatabase::addApplicationFont(":fonts/19287");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont titleFont(family);
    titleFont.setPointSize(60);
    ui->labelTitle->setFont(titleFont);

    id = QFontDatabase::addApplicationFont(":fonts/20322");
    family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont labelFont(family);
    labelFont.setPointSize(22);
    ui->labelCount->setFont(labelFont);
    ui->labelShow->setFont(labelFont);
    labelFont.setPointSize(20);
    ui->labelExit->setFont(labelFont);
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
    clearResultViews();
    int id = fork;
    int maxStepCount = getMaxStepCount() + 1;

    for (int step = 0; step < maxStepCount; step++){
        for (ResultRecord* record : client->result) {
            if ((record->playerID == id) && (record->gameStep == step)) {
                delay(2);
                if (!record->image.isNull()) { //// image
                    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(record->image));
                    QGraphicsScene* scene = new QGraphicsScene;
                    scene->addItem(item);
                    QGraphicsView* view = new QGraphicsView;
                    view->setMinimumSize(QSize(943, 594));
                    view->setScene(scene);
                    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                    view->show();
                    ui->resultVbox->addWidget(view);
                    ui->resultAreaWidget->resize(ui->resultAreaWidget->geometry().width(), ui->resultAreaWidget->geometry().height() + 600);
                } else {  //// message
                    QLabel* label = new QLabel;
                    label->setText(record->message);
                    label->setMinimumSize(QSize(600, 30));
                    int id = QFontDatabase::addApplicationFont(":fonts/20322");
                    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
                    QFont titleFont(family);
                    titleFont.setPointSize(24);
                    label->setFont(titleFont);
                    label->show();
                    ui->resultVbox->addWidget(label);
                    ui->resultAreaWidget->resize(ui->resultAreaWidget->geometry().width(), ui->resultAreaWidget->geometry().height() + 30);
                }
            }
        }
        id = getPrevId(id, maxStepCount - 1);
    }
}

void ResultWindow::delay(int sec){
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int ResultWindow::getPrevId(int currentId, int maxId){
    int result = currentId;
    if (currentId == 0){
        result = maxId;
    } else {
        result--;
    }
    return result;
}

int ResultWindow::getMaxStepCount(){
    int maxStepCount = 0;
    for (ResultRecord* record : client->result) {
        if (record->gameStep > maxStepCount){
            maxStepCount = record->gameStep;
        }
    }
    return maxStepCount;
}

void ResultWindow::clearResultViews(){
    int count = ui->resultVbox->count();
    for (int i = 0; i < count; i++){
        QWidget* widget = ui->resultVbox->itemAt(0)->widget();
        ui->resultVbox->removeWidget(widget);
        widget->setParent(NULL);
        widget->deleteLater();
    }
    ui->resultAreaWidget->resize(ui->resultAreaWidget->geometry().width(), 0);
}


void ResultWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

