#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <client.h>


QT_BEGIN_NAMESPACE
namespace Ui {class ResultWindow; }
QT_END_NAMESPACE

class ResultWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWindow(QWidget *parent = nullptr, Client *client = nullptr);
    ~ResultWindow();

private:
    Ui::ResultWindow *ui;
    Client *client;
    void initUi();
    int fork = 0;
    void showIndicator(int number);
    void clearResultViews();
    int getPrevId(int currentId, int maxId);
    int getMaxStepCount();

public slots:


private slots:
    void btnExitClick();
    void btnIncCountClick();
    void btnDecCountClick();
    void updateResultList();


signals:
    void toMenuWindow();


};

#endif // RESULTWINDOW_H
