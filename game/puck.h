#ifndef PUCK_H
#define PUCK_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include "vectorxy.h"

class Puck: public QObject,public QGraphicsEllipseItem
{

public:
    double radius;

    QBrush brush;

    VectorXY * velocity;
    VectorXY * acceleration;

    Puck();
    Puck(qreal radius, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y);
    void setXVelocity(qreal xv);
    void setYVelocity(qreal yv);
    void setXAcceleration(qreal xa);
    void setYAcceleration(qreal ya);

};


#endif // PUCK_H



