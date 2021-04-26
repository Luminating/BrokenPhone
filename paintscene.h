#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class PaintScene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PaintScene(QObject *parent = 0);
    ~PaintScene();

public:
    void setPenColor(QColor color);
    void setBackgroundColor(QColor color);

private:

    QPointF previousPoint;
    QColor currentPenColor;


private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

};

#endif // PAINTSCENE_H
