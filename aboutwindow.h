#ifndef ABOUTFRAME_H
#define ABOUTFRAME_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui {class AboutWindow; }
QT_END_NAMESPACE

class AboutWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AboutWindow(QWidget *parent = nullptr);
    ~AboutWindow();

private:
    Ui::AboutWindow *ui;
    void initUi();

public slots:


private slots:
    void btnExitClick();


signals:
    void toMenuWindow();


};

#endif // ABOUTFRAME_H
