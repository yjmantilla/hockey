#ifndef ACCELERATOR_H
#define ACCELERATOR_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include "vectorxy.h"

/*
 * Mass gotta be around the order of 1000 to have an observable effect
 * Accelerators may be created inside or outside the field.
 * Negative Mass would indicate a repulsor, positive an attractor.
 * Color could indicate wether it attracts or repulses
 */
class Accelerator:public QGraphicsEllipseItem
{
public:
    QBrush brush;

    VectorXY * velocity;

    qreal mass;
    qreal radius;


    Accelerator();
    Accelerator(qreal radius, qreal mass, Qt::BrushStyle brush, Qt::GlobalColor color, qreal x, qreal y, qreal vx, qreal vy);
    void paintAccelerator();

    ~Accelerator();
};

#endif // ACCELERATOR_H
