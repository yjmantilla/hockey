#include "vectorxy.h"

VectorXY::VectorXY()
{

}

VectorXY::VectorXY(qreal x, qreal y)
{
    this->x = x;
    this->y = y;
    return;
}

void VectorXY::setVector(qreal x, qreal y)
{
    this->x = x;
    this->y = y;
    return;
}

void VectorXY::setX(qreal x)
{
    this->x = x;
    return;
}

void VectorXY::setY(qreal y)
{
    this->y = y;
}

qreal VectorXY::getX()
{
    return this->x;
}

qreal VectorXY::getY()
{
    return this->y;
}
