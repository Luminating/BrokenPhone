#include "paintscene.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{
    currentPenColor = QColorConstants::Black;
    currentLineWidth = 10;
}

PaintScene::~PaintScene()
{

}

void PaintScene::setPenColor(QColor color){
    currentPenColor = color;
}

void PaintScene::setLineWidth(int width){
    currentLineWidth = width;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // При нажатии кнопки мыши отрисовываем эллипс
    addEllipse(event->scenePos().x() - currentLineWidth/2,
               event->scenePos().y() - currentLineWidth/2,
               currentLineWidth,
               currentLineWidth,
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
            QPen(currentPenColor,currentLineWidth,Qt::SolidLine,Qt::RoundCap));
    // Обновляем данные о предыдущей координате
    previousPoint = event->scenePos();
}
