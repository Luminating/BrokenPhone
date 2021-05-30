#include "paintwindow.h"
#include "ui_paintwindow.h"
#include <QIcon>
#include <QButtonGroup>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QBuffer>
#include <QDir>
#include <QTime>

static const int GameStepInterval = 1 * 60; /// 1 min

PaintWindow::PaintWindow(QWidget *parent, Client *client) : QWidget(parent), ui(new Ui::PaintWindow)
{
    ui->setupUi(this);
    this->client = client;
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
    connect(ui->btnReady, SIGNAL(clicked()), this, SLOT(btnReadyClick()));
    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(btnNextClick()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(btnPrevClick()));
    connect(ui->btnSetFoneColor, SIGNAL(clicked()), this, SLOT(btnSetFoneColorClick()));
    connect(ui->sldLineWidth, SIGNAL(valueChanged(int)), this, SLOT(sldSetLineWidth(int)));
    connect(ui->btnSetFoneColor, SIGNAL(pressed()), this, SLOT(btnSetFoneColorClick()));
    connect(&indicatorTimer, SIGNAL(timeout()), this, SLOT(startTimeIndicator()));

    connect(client, SIGNAL(startGame()), this, SLOT(onStartGame()));
    connect(client, SIGNAL(nextGameStep()), this, SLOT(onNextGameStep()));
    connect(client, SIGNAL(gameShowImage(QByteArray)), this, SLOT(onGameShowImage(QByteArray)));
    connect(client, SIGNAL(gameShowMessage(QString)), this, SLOT(onGameShowMessage(QString)));
    connect(client, SIGNAL(endGame()), this, SLOT(onEndGame()));
    connect(client, SIGNAL(stopGameError(QString)), this, SLOT(onStopGameError(QString)));

    stepSecondsLeft = GameStepInterval;
    indicatorTimer.setInterval(1 * 1000);

    graphicsView = new QGraphicsView();
    graphicsView->setMinimumSize(QSize(943, 594));
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    paintscene = new PaintScene();       // Инициализируем графическую сцену
    paintscene->setSceneRect(0, 0, graphicsView->width(), graphicsView->height());
    graphicsView->setScene(paintscene);  // Устанавливаем графическую сцену
    currBackgroundColor = QColorConstants::White;
    graphicsView->setStyleSheet("background-color: rgba(255, 255, 255, 1);");
    ui->frameLayout->addWidget(graphicsView);

    ui->sldLineWidth->setStyleSheet("QSlider::groove:horizontal {"

                                        "background: url(:slider_groove) 100% 100% no-repeat;"
                                        "height: 32px;"
                                        "margin: 13px 0px 13px 0px;}"

                                    "QSlider::handle:horizontal {"
                                        "background: url(:slider_handle);"
                                        "width: 32px;"
                                        "margin: -13px 38px -13px 38px;}"
                                    );

    QImage imgActive(":imgBtnColorActive");
    QImage imgInactive(":imgBtnColorInactive");

    colorList[0] = QColor(51,51,51);//QColorConstants::Black;
    colorList[1] = QColor(51,102,153);//QColorConstants::DarkBlue;
    colorList[2] = QColor(255,102,102);//QColorConstants::DarkMagenta;
    colorList[3] = QColor(204,51,51);//QColorConstants::DarkRed;
    colorList[4] = QColor(51,153,153);//QColorConstants::DarkGreen;
    colorList[5] = QColor(255,153,102);//QColorConstants::DarkYellow;
    colorList[6] = QColor(255,255,204);//QColorConstants::White;
    colorList[7] = QColor(153,204,255);//QColorConstants::Blue;
    colorList[8] = QColor(204,204,153);//QColorConstants::Red;
    colorList[9] = QColor(204,204,204);//QColorConstants::Gray;
    colorList[10] = QColor(153,204,102);//QColorConstants::Green;
    colorList[11] = QColor(255,204,51);//QColorConstants::Yellow;

    QButtonGroup* ColorGroup = new QButtonGroup(ui->groupBoxColor);
    ColorGroup->setExclusive(true);
    for(int i = 0; i < 12; i++){
        btnListColor[i] = new QPushButton(ui->groupBoxColor);
        initColorButton(btnListColor[i],imgActive, imgInactive, colorList[i]);
        ColorGroup->addButton(btnListColor[i], i);
        if (i <6) {
            ui->gridLayoutColor->addWidget(btnListColor[i],i,0);
        } else {
            ui->gridLayoutColor->addWidget(btnListColor[i],i-6,1);
        }
        if (i==0){
            btnListColor[0]->setChecked(true);
            currentPenColor = colorList[0];
            paintscene->setPenColor(colorList[0]);
        }
    }
    connect(ColorGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotBtnColorClicked(int)));

    ui->btnSetFoneColor->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnNext->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnPrev->setStyleSheet("QPushButton:pressed {background: url(:btnBlackActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBlackInactive);}");
    ui->btnReady->setStyleSheet("QPushButton:pressed {background: url(:btnBigRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnBigRedInactive);}");
}

void PaintWindow::slotBtnColorClicked(int number){
    if (btnListColor[number]->isChecked()){
        currentPenColor = colorList[number];
        paintscene->setPenColor(colorList[number]);
    }
}

void PaintWindow::sldSetLineWidth(int numberWidth){
    int lineWidth[] = {6, 10, 16, 20, 26};
    paintscene->setLineWidth(lineWidth[numberWidth -1]);
}

void PaintWindow::initColorButton(QPushButton* button, QImage imgActive, QImage imgInactive, QColor color){
    QIcon icon;
    button->setCheckable(true);
    button->setFixedSize(70, 70);
    button->setIconSize(QSize(68, 68));
    button->setFlat(true);
    icon.addPixmap(QPixmap::fromImage(setImageColor(imgActive, color)), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap::fromImage(setImageColor(imgInactive, color)), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}


QImage PaintWindow::setImageColor(QImage image, QColor color){
    //int r,g,b;
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            if (image.pixelColor(i, j) == QColorConstants::Black){
                image.setPixelColor(i, j, color);
            }
        }
    }

    return image;
}

PaintWindow::~PaintWindow()
{
    delete ui;
}

void PaintWindow::btnExitClick(){ //// del
    this->close();
    emit toMenuWindow();
}

void PaintWindow::delay(int sec){
    QTime dieTime= QTime::currentTime().addSecs(sec);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void PaintWindow::btnSetFoneColorClick(){
    graphicsView->setStyleSheet("background-color: rgba(" +  QString::number(currentPenColor.red()) + "," +
                                                                    QString::number(currentPenColor.green()) + "," +
                                                                    QString::number(currentPenColor.blue()) + "," +
                                                                    "1);");
    currBackgroundColor = currentPenColor;

}


void PaintWindow::btnNextClick(){
    QList<QGraphicsItem*> element = popGraphicsItemStack();
    for (int i = 0; i < element.size(); i++) {
        paintscene->addItem(element[i]);
    }
}

void PaintWindow::btnPrevClick(){
    QList<QGraphicsItem*> items = paintscene->items();
    QList<QGraphicsItem*> element;
    for (int i = 0; i < items.size(); i++){
        QGraphicsItem* item = items[i];
        paintscene->removeItem(item);
        element.push_front(item);
        if ((item->type() == QGraphicsEllipseItem::Type)){
            break;
        }
    }
    pushGraphicsItemStack(element);
}

void PaintWindow::pushGraphicsItemStack(QList<QGraphicsItem*> element){
    int countElementGraphicsItemStack = 0;
    for (int i = 0; i < element.size(); i++){
        graphicsItemStack.push_front(element[i]);
    }
    for (int i = 0; i < graphicsItemStack.size(); i++) {
        if ((graphicsItemStack[i]->type() == QGraphicsEllipseItem::Type)) {
            countElementGraphicsItemStack++;
        }
    }
    // delete overflow elements
    if (countElementGraphicsItemStack > maxElementGraphicsItemStack) {
        if (!graphicsItemStack.isEmpty()) {
            QGraphicsItem* item = graphicsItemStack.last();
            graphicsItemStack.removeLast();
            delete item;
        }
        while(!graphicsItemStack.isEmpty()){
            if (graphicsItemStack.last()->type() == QGraphicsEllipseItem::Type) {
                break;
            }
            QGraphicsItem* item = graphicsItemStack.last();
            graphicsItemStack.removeLast();
            delete item;
        }
    }
}


QList<QGraphicsItem*> PaintWindow::popGraphicsItemStack(){
    QList<QGraphicsItem*> element;
    while(!graphicsItemStack.isEmpty()){
        element.push_front(graphicsItemStack.first());
        graphicsItemStack.removeFirst();
        if (element[0]->type() == QGraphicsEllipseItem::Type) {
            break;
        }
    }
    return element;
}

void PaintWindow::btnReadyClick(){
    turnEnd();
}

void PaintWindow::turnEnd(){
    if (!isPressedTurnEnd){
        if ((client->currentGameStep % 2) == 0) { /// step 0,2,4,.. - text , step 1,3,5, - image
            QString message = "gameMessageResult\n" + ui->lineEdit->text() + "\n" + QString::number(client->currentGameStep) + "\n" + QString::number(client->userId);
            client->sendMessageTo(client->roomname.split("\n").at(1), client->roomname.split("\n").at(2), message);
            ui->labMessage->setText("Сообщение отправлено");
        } else {
            QImage image(paintscene->sceneRect().size().toSize(), QImage::Format_ARGB32);
            image.fill(currBackgroundColor);
            QPainter painter(&image);
            paintscene->render(&painter);

            QByteArray array;
            QBuffer buffer(&array);
            buffer.open(QIODevice::WriteOnly);
            image.save(&buffer, "PNG");
            array = client->commandToByteArray("gameImageResult\n" + QString::number(client->currentGameStep) + "\n" + QString::number(client->userId), array);
            client->sendByteArrayTo(client->roomname.split("\n").at(1), client->roomname.split("\n").at(2), array);
            ui->labMessage->setText("Сообщение отправлено");
        }
    }
    isPressedTurnEnd = true;
}


void PaintWindow::startTimeIndicator(){
    stepSecondsLeft--;
    int timeSec = stepSecondsLeft;
    int min_1 = (timeSec / 60) / 10;
    ui->labIndicator1_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(min_1) + ");}");
    int min_0 = (timeSec / 60) % 10;
    ui->labIndicator2_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(min_0) + ");}");
    int sec_1 = (timeSec % 60) / 10;
    ui->labIndicator4_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(sec_1) + ");}");
    int sec_0 = (timeSec % 60) % 10;
    ui->labIndicator5_num->setStyleSheet("QLabel{background: url(:indicator_" + QString::number(sec_0) + ");}");

    indicatorTimer.setInterval(1 * 1000);
    if (stepSecondsLeft > 0) {
        indicatorTimer.start();
    } else {
        indicatorTimer.stop();
        turnEnd();
    }
}

void PaintWindow::onStartGame(){
    client->currentGameStep = 0;
    stepSecondsLeft = GameStepInterval;
    isPressedTurnEnd = false;

    QImage image = QImage(":/startImageNew");
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(item);
    graphicsView->setScene(scene);
    ui->lineEdit->setText("");
    ui->lineEdit->setReadOnly(false);
    startTimeIndicator();
}

void PaintWindow::onNextGameStep(){
    stepSecondsLeft = GameStepInterval;
    isPressedTurnEnd = false;
    startTimeIndicator();
}


void PaintWindow::onGameShowImage(const QByteArray &array){
    QImage image;
    image.loadFromData(array, "PNG");

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addItem(item);
    graphicsView->setScene(scene);
    if (client->currentGameStep == 0) {
        ui->labMessage->setText("Начинаем, введите фразу");
    } else {
        ui->labMessage->setText("Напишите, что видите");
    }
    ui->lineEdit->setText("");
    ui->lineEdit->setReadOnly(false);
}


void PaintWindow::onGameShowMessage(const QString &message){
    graphicsView->setScene(paintscene);
    if (client->currentGameStep == 0) {
        ui->labMessage->setText(message);
    } else {
        ui->labMessage->setText("Нарисуйте: " + message);
    }
    ui->lineEdit->setText("");
    ui->lineEdit->setReadOnly(true);
}

void PaintWindow::onEndGame(){
    this->close();
}

void PaintWindow::onStopGameError(const QString &message){
    ui->labMessage->setText(message);
    delay(5);
    this->close();
    emit toMenuWindow();
}
