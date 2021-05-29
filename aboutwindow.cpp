#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QFontDatabase>

AboutWindow::AboutWindow(QWidget *parent) : QWidget(parent), ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    initUi();
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
}

void AboutWindow::initUi(){
    ui->label->setText("\n\nДанное программое средство является\n"
                       "курсовым проектом студента 2-го курса\n"
                       "Гордеюка Павла Александровича.\n\n\n"
                       "ФКСиС, ПОИТ\n"
                       "Минск, БГУИР, 2021");
    int id = QFontDatabase::addApplicationFont(":fonts/20322");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont titleFont(family);
    titleFont.setPointSize(26);
    ui->label->setFont(titleFont);
    titleFont.setPointSize(20);
    ui->labelExit->setFont(titleFont);
    id = QFontDatabase::addApplicationFont(":fonts/19287");
    family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont menuFont(family);
    menuFont.setPointSize(30);
    ui->labelTitle->setFont(menuFont);

    ui->btnExit->setStyleSheet("QPushButton:pressed {background: url(:btnRedActive);}"
                                           "QPushButton {border: none;"
                                           "background: url(:btnRedInactive);}");
}

AboutWindow::~AboutWindow(){
    delete ui;
}


void AboutWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}

