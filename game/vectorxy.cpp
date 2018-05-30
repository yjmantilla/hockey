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

void VectorXY::setVectorByMagnitudeAngle(qreal magnitude, qreal angle)
{
    this->setX(magnitude * qCos(angle));
    this->setY(magnitude * qSin(angle));
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

qreal VectorXY::getMagnitude()
{
    return qSqrt(qPow(this->getX(),2) + qPow(this->getY(),2));
}

qreal VectorXY::getAngle()
{
    return qAtan2(this->getY(),this->getX());
}

qreal VectorXY::getMagnitudeSquared()
{
    return (qPow(this->getX(),2) + qPow(this->getY(),2));
}
