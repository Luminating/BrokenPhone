#include "paintwindow.h"
#include "ui_paintwindow.h"

PaintWindow::PaintWindow(QWidget *parent) : QWidget(parent), ui(new Ui::PaintWindow)
{
    ui->setupUi(this);
    connect(ui->btnExit, SIGNAL(clicked()), this, SLOT(btnExitClick()));
}

PaintWindow::~PaintWindow()
{
    delete ui;
}

void PaintWindow::btnExitClick(){
    this->close();
    emit toMenuWindow();
}
