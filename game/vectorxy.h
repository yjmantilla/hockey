#ifndef VECTORXY_H
#define VECTORXY_H

#include <QtGlobal>

class VectorXY
{
public:
    qreal x;
    qreal y;


    VectorXY();
    VectorXY(qreal x, qreal y);
    void setVector(qreal x ,qreal y);
    void setX(qreal x);
    void setY(qreal y);
    qreal getX();
    qreal getY();

};

#endif // VECTORXY_H
