#ifndef VECTORXY_H
#define VECTORXY_H

#include <QtGlobal>
#include <QtMath>

/*
 * VectorXY Class
 *
 * This class is a general purpose abstraction of a bidimensional vector.
 *
*/
class VectorXY
{
public:
/* Attributes */
    qreal x;
    qreal y;

/* Methods */
    /* Constructor */
    VectorXY();
    VectorXY(qreal x, qreal y);

    /* Functionality */
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
