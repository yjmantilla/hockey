#include "puck.h"

Puck::Puck()
{

}

Puck::Puck(qreal radius, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y)
{
    this->velocity = new VectorXY();
    this->acceleration = new VectorXY();
    this->radius=radius;
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush);

}

Puck::~Puck()
{
    delete this->velocity;
    delete this->acceleration;
}
