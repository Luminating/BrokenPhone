#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <paintwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private:
    Ui::MenuWindow *ui;
    PaintWindow* paintWindow;

private slots:
    void btnCreateRoomClick();


};
#endif // MAINWINDOW_H
