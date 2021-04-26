#ifndef PAINTWINDOW_H
#define PAINTWINDOW_H
#include <QWidget>
#include <QPushButton>
#include <paintscene.h>

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
    QImage setImageColor(QImage image, QColor color);
    PaintScene *scene;
    QPushButton *buttonList[16];
    QColor colorList[16];
    void initColorButton(QPushButton* button, QImage imgActive, QImage imgInactive, QColor color);

private slots:
    void btnExitClick();
    void btnNextClick();
    void btnPrevClick();
    void slotBtnColorClicked(int number);

signals:
    void toMenuWindow();


};

#endif // PAINTWINDOW_H
