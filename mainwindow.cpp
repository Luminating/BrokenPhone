#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setText("Broken Phone");

    this->showFullScreen();
    paintWindow = new PaintWindow();
    connect(paintWindow, &PaintWindow::toMenuWindow, this, &MainWindow::showFullScreen);
    connect(ui->btnPlay, SIGNAL(clicked()), this, SLOT(btnPlayClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::btnPlayClick(){
        paintWindow->showFullScreen();
        this->close();
}
