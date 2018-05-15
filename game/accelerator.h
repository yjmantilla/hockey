#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <QGraphicsEllipseItem>
#include <QBrush>


class Accelerator:public QGraphicsEllipseItem
{
public:
    double mass;
    double radius;
    double xVelocity=0;
    double yVelocity=0;


    Accelerator();
};

#endif // ACCELERATOR_H
