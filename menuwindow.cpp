#include "menuwindow.h"
#include "ui_menuwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    this->showFullScreen();
    paintWindow = new PaintWindow(nullptr, new Client());
    connect(paintWindow, &PaintWindow::toMenuWindow, this, &MenuWindow::showFullScreen);
    connect(ui->btnCreateRoom, SIGNAL(clicked()), this, SLOT(btnCreateRoomClick()));
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::btnCreateRoomClick(){
        paintWindow->showFullScreen();
        this->close();
}
