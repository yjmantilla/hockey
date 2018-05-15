#include "puck.h"

Puck::Puck()
{

}

Puck::Puck(double radius, Qt::BrushStyle style, Qt::GlobalColor color, double x, double y)
{
    this->radius=radius;
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush);

}

void Puck::setXVelocity(qreal xv)
{
    this->xVelocity = xv;
    return;
}

void Puck::setYVelocity(qreal yv)
{
    this->yVelocity = yv;
    return;
}

void Puck::setXAcceleration(qreal xa)
{
    this->xAcceleration = xa;
    return;
}

void Puck::setYAcceleration(qreal ya)
{
    this->yAcceleration = ya;
    return;
}
