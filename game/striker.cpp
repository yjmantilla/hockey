#include "striker.h"

Striker::Striker(qreal x, qreal y, qreal width, qreal height, qreal vel,Qt::BrushStyle style, Qt::GlobalColor color)
{
    /* Set up physics attributes */
    this->velocity = new VectorXY(vel,0);
    this->setRestitution(1);

    /* Appearance */
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

void Striker::negateVelocity()
{
    this->velocity->setX(this->velocity->getX()*-1);
    return;
}

Striker::~Striker()
{
    delete this->velocity;
}
