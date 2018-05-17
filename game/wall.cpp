#include "wall.h"

Wall::Wall()
{

}

Wall::Wall(qreal x1, qreal y1, qreal x2, qreal y2, Qt::GlobalColor color)
{
    this->setLine(x1,y1,x2,y2);
    this->setPen(QPen(color, 7, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void Wall::setRestitution(qreal r)
{
    this->restitution = r;
    return;
}

qreal Wall::getRestitution()
{
    return this->restitution;
}
