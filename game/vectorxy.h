#ifndef VECTORXY_H
#define VECTORXY_H

#include <QtGlobal>
#include <QtMath>

class VectorXY
{
public:
    qreal x;
    qreal y;


    VectorXY();
    VectorXY(qreal x, qreal y);
    void setVector(qreal x ,qreal y);
    void setVectorByMagnitudeAngle(qreal magnitude, qreal angle);
    void setX(qreal x);
    void setY(qreal y);
    qreal getX();
    qreal getY();
    qreal getMagnitude();
    qreal getAngle();
    qreal getMagnitudeSquared();

};

#endif // VECTORXY_H
