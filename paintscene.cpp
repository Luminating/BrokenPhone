#include "paintscene.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{
    currentPenColor = QColorConstants::Black;
}

PaintScene::~PaintScene()
{

}

void PaintScene::setPenColor(QColor color){
    currentPenColor = color;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // При нажатии кнопки мыши отрисовываем эллипс
    addEllipse(event->scenePos().x() - 5,
               event->scenePos().y() - 5,
               10,
               10,
               QPen(Qt::NoPen),
               QBrush(currentPenColor));
    // Сохраняем координаты точки нажатия
    previousPoint = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Отрисовываем линии с использованием предыдущей координаты
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(currentPenColor,10,Qt::SolidLine,Qt::RoundCap));
    // Обновляем данные о предыдущей координате
    previousPoint = event->scenePos();
}
