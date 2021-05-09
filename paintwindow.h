#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H
#include <QWidget>
#include <QPushButton>
#include <paintscene.h>
#include <client.h>

QT_BEGIN_NAMESPACE
namespace Ui {class PaintWindow; }
QT_END_NAMESPACE

class PaintWindow : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWindow(QWidget *parent = nullptr, Client *client = nullptr);
    ~PaintWindow();

private:
    Ui::PaintWindow *ui;
    QImage setImageColor(QImage image, QColor color);
    PaintScene *scene;
    QPushButton *btnListColor[12];
    QColor colorList[12];
    QColor currentPenColor;
    QColor currBackgroundColor;
    Client *client;

    QList<QGraphicsItem*> graphicsItemStack;
    int maxElementGraphicsItemStack = 10;
    void pushGraphicsItemStack(QList<QGraphicsItem*> element);
    QList<QGraphicsItem*> popGraphicsItemStack();

    void initColorButton(QPushButton* button, QImage imgActive, QImage imgInactive, QColor color);
    void turnEnd();

private slots:
    void btnExitClick();
    void btnNextClick();
    void btnPrevClick();
    void btnSetFoneColorClick();
    void slotBtnColorClicked(int number);
    void sldSetLineWidth(int numberWidth);
    void btnReadyClick();

signals:
    void toMenuWindow();


};

#endif // PAINTWINDOW_H
