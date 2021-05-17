#include "paintwindow.h"
#include "ui_paintwindow.h"
#include <QIcon>
#include <QButtonGroup>
#include <QGraphicsItem>
#include <QBuffer>

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

    scene = new PaintScene();       // Инициализируем графическую сцену
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    currBackgroundColor = QColorConstants::White;
    ui->graphicsView->setStyleSheet("background-color: rgba(255, 255, 255, 1);");


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

    colorList[0] = QColorConstants::Black;
    colorList[1] = QColorConstants::DarkBlue;
    colorList[2] = QColorConstants::DarkMagenta;
    colorList[3] = QColorConstants::DarkRed;
    colorList[4] = QColorConstants::DarkGreen;
    colorList[5] = QColorConstants::DarkYellow;
    colorList[6] = QColorConstants::White;
    colorList[7] = QColorConstants::Blue;
    colorList[8] = QColorConstants::Red;
    colorList[9] = QColorConstants::Gray;
    colorList[10] = QColorConstants::Green;
    colorList[11] = QColorConstants::Yellow;


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
            scene->setPenColor(colorList[0]);
        }
    }
    connect(ColorGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotBtnColorClicked(int)));

    //QIcon icon;
    //ui->btnSetFoneColor->setFixedSize(95, 95);
    //ui->btnSetFoneColor->setIconSize(QSize(95, 95));
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
        scene->setPenColor(colorList[number]);
    }
}

void PaintWindow::sldSetLineWidth(int numberWidth){
    int lineWidth[] = {6, 10, 16, 20, 26};
    scene->setLineWidth(lineWidth[numberWidth -1]);
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
            /*
            if (image.pixelColor(i,j).alpha() > 0) {
                r = (image.pixelColor(i,j).red() + color.red()) > 255 ? 255 : image.pixelColor(i,j).red() + color.red();
                g = (image.pixelColor(i,j).green() + color.green()) > 255 ? 255 : image.pixelColor(i,j).green() + color.green();
                b = (image.pixelColor(i,j).blue() + color.blue()) > 255 ? 255 : image.pixelColor(i,j).blue() + color.blue();
                image.setPixelColor(i,j, QColor(r, g, b));
            }
            */
        }
    }

    return image;
}

PaintWindow::~PaintWindow()
{
    delete ui;
}

void PaintWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

void PaintWindow::btnSetFoneColorClick(){
    ui->graphicsView->setStyleSheet("background-color: rgba(" +  QString::number(currentPenColor.red()) + "," +
                                                                    QString::number(currentPenColor.green()) + "," +
                                                                    QString::number(currentPenColor.blue()) + "," +
                                                                    "1);");
    currBackgroundColor = currentPenColor;

}


void PaintWindow::btnNextClick(){
   // ui->labMessage->setText(QString::number(graphicsItemStack.size()));
    QList<QGraphicsItem*> element = popGraphicsItemStack();
    //ui->labMessage->setText(QString::number(element.size()));
    for (int i = 0; i < element.size(); i++) {
        scene->addItem(element[i]);
    }
}

void PaintWindow::btnPrevClick(){
    //ui->labMessage->setText(QString::number(scene->items().size()));
    QList<QGraphicsItem*> items = scene->items();
    QList<QGraphicsItem*> element;
    for (int i = 0; i < items.size(); i++){
        QGraphicsItem* item = items[i];
        scene->removeItem(item);
        element.push_front(item);
        if ((item->type() == QGraphicsEllipseItem::Type)){
            break;
        }
    }
    pushGraphicsItemStack(element);
}

void PaintWindow::pushGraphicsItemStack(QList<QGraphicsItem*> element){
    int countElementGraphicsItemStack = 0;
    //ui->labMessage->setText(QString::number(element.size()));
    for (int i = 0; i < element.size(); i++){
        graphicsItemStack.push_front(element[i]);
    }
    for (int i = 0; i < graphicsItemStack.size(); i++) {
        if ((graphicsItemStack[i]->type() == QGraphicsEllipseItem::Type)) {
            countElementGraphicsItemStack++;
        }
    }
    //ui->labMessage->setText(QString::number(countElementGraphicsItemStack));
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
    QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
    image.fill(currBackgroundColor);
    QPainter painter(&image);
    scene->render(&painter);
    image.save("picture.png");

    QByteArray array;
    QBuffer buffer(&array);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    client->sendByteArrayTo(client->roomname.split("\n").at(1), client->roomname.split("\n").at(2), array);
}

