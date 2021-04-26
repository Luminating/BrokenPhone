#include "paintwindow.h"
#include "ui_paintwindow.h"
#include <QIcon>
#include <QButtonGroup>
#include <QGraphicsItem>

PaintWindow::PaintWindow(QWidget *parent) : QWidget(parent), ui(new Ui::PaintWindow)
{
    ui->setupUi(this);
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));

    connect(ui->btnNext, SIGNAL(clicked()), this, SLOT(btnNextClick()));
    connect(ui->btnPrev, SIGNAL(clicked()), this, SLOT(btnPrevClick()));
    scene = new PaintScene();       // Инициализируем графическую сцену
    ui->graphicsView->setScene(scene);  // Устанавливаем графическую сцену
    ui->graphicsView->setStyleSheet("background-color: rgba(50, 50, 255, 1);");

    QImage imgActive(":imgBtnColorActive");
    QImage imgInactive(":imgBtnColorInactive");

    colorList[0] = QColorConstants::Black;
    colorList[1] = QColorConstants::Blue;
    colorList[2] = QColorConstants::DarkCyan;
    colorList[3] = QColorConstants::Gray;
    colorList[4] = QColorConstants::Green;
    colorList[5] = QColorConstants::DarkYellow;
    colorList[6] = QColorConstants::DarkRed;
    colorList[7] = QColorConstants::Red;
    colorList[8] = QColorConstants::White;
    colorList[9] = QColorConstants::Magenta;
    colorList[10] = QColorConstants::DarkGreen;
    colorList[11] = QColorConstants::DarkMagenta;
    colorList[12] = QColorConstants::Yellow;
    colorList[13] = QColorConstants::DarkBlue;
    colorList[14] = QColorConstants::DarkGray;
    colorList[15] = QColorConstants::Cyan;

    QButtonGroup* colorGroup = new QButtonGroup(ui->groupBox);
    colorGroup->setExclusive(true);

    for(int i = 0; i < 16; i++){
        buttonList[i] = new QPushButton(ui->groupBox);
        initColorButton(buttonList[i],imgActive, imgInactive, colorList[i]);
        colorGroup->addButton(buttonList[i], i);
        if (i < 8) {
            ui->gridLayout->addWidget(buttonList[i],i,0);
        } else {
            ui->gridLayout->addWidget(buttonList[i],i-8,1);
        }
    }
    connect(colorGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotBtnColorClicked(int)));
}

void PaintWindow::slotBtnColorClicked(int number){
    if (buttonList[number]->isChecked()){
        scene->setPenColor(colorList[number]);
    }
}

void PaintWindow::initColorButton(QPushButton* button, QImage imgActive, QImage imgInactive, QColor color){
    QIcon icon;
    button->setCheckable(true);
    button->setFixedSize(60, 60);
    button->setIconSize(QSize(56, 56));
    button->setFlat(true);
    icon.addPixmap(QPixmap::fromImage(setImageColor(imgActive, color)), QIcon::Normal, QIcon::On);
    icon.addPixmap(QPixmap::fromImage(setImageColor(imgInactive, color)), QIcon::Normal, QIcon::Off);
    button->setIcon(icon);
    button->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
}


QImage PaintWindow::setImageColor(QImage image, QColor color){
    int r,g,b;
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            if (image.pixelColor(i,j).alpha() > 0) {
                r = (image.pixelColor(i,j).red() + color.red()) > 255 ? 255 : image.pixelColor(i,j).red() + color.red();
                g = (image.pixelColor(i,j).green() + color.green()) > 255 ? 255 : image.pixelColor(i,j).green() + color.green();
                b = (image.pixelColor(i,j).blue() + color.blue()) > 255 ? 255 : image.pixelColor(i,j).blue() + color.blue();
                image.setPixelColor(i,j, QColor(r, g, b));
            }
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


void PaintWindow::btnNextClick(){

}

void PaintWindow::btnPrevClick(){
    ui->labMessage->setText(QString::number(scene->items().size()));
    QList<QGraphicsItem*> items = scene->items();
    bool isBreak = false;
    for (int i = 0; i < items.size(); i++){
        QGraphicsItem* item = items[i];
        isBreak = (item->type() == QGraphicsEllipseItem::Type);
        delete item;
        if (isBreak){
            break;
        }
    }
}

