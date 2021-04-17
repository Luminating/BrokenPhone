#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {class PaintWindow; }
QT_END_NAMESPACE

class PaintWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWindow(QWidget *parent = nullptr);
    ~PaintWindow();

private:
    Ui::PaintWindow *ui;

private slots:
    void btnExitClick();


signals:
    void toMenuWindow();

};

#endif // PAINTWINDOW_H
