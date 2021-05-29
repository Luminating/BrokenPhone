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
    PaintScene *paintscene;
    QGraphicsView *graphicsView;
    QPushButton *btnListColor[12];
    QColor colorList[12];
    QColor currentPenColor;
    QColor currBackgroundColor;
    Client *client;

    QList<QGraphicsItem*> graphicsItemStack;
    int maxElementGraphicsItemStack = 10;
    void pushGraphicsItemStack(QList<QGraphicsItem*> element);
    QList<QGraphicsItem*> popGraphicsItemStack();
    int stepSecondsLeft;
    QTimer indicatorTimer;
    bool isPressedTurnEnd;
    void delay(int sec);
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
    void onStartGame();
    void onNextGameStep();
    void startTimeIndicator();
    void onGameShowImage(const QByteArray &array);
    void onGameShowMessage(const QString &message);
    void onEndGame();
    void onStopGameError(const QString &message);

signals:
    void toMenuWindow();


};

#endif // PAINTWINDOW_H
