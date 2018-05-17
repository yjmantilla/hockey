#include "striker.h"

Striker::Striker(qreal x, qreal y, qreal width, qreal height, Qt::BrushStyle style, Qt::GlobalColor color)
{
    this->velocity = new VectorXY(3,0);
    this->setRect(x,y,width,height);
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setBrush(this->brush);
}
