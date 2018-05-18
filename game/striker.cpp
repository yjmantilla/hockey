#include "striker.h"

Striker::Striker(qreal x, qreal y, qreal width, qreal height, Qt::BrushStyle style, Qt::GlobalColor color)
{
    this->velocity = new VectorXY(4,0);
    this->setRect(x,y,width,height);
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setBrush(this->brush);
}

void Striker::setRestitution(qreal r)
{
    this->restitution = r;
    return;
}

qreal Striker::getRestitution()
{
    return this->restitution;
}

void Striker::setWidth(qreal width)
{
    this->setRect(0,0,width,this->rect().height());
    return;
}

Striker::~Striker()
{
    delete this->velocity;
}
