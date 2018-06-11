#ifndef ACCELERATOR_H
#define ACCELERATOR_H

/* Libraries */
#include <QGraphicsEllipseItem>
#include <QBrush>
#include "vectorxy.h"

/*
 * Accelerator Class
 *
 * Represents an accelerator.
 *
 * It either attracts or repels the puck.
 *
 * Visually is a circle with a defined radius.
 * It is colored based on its nature:
 * Red: Attracts
 * Blue: Repels
 *
 * It moves with a constant velocity on both axis, no matter other bodies on the field.
*/

class Accelerator:public QGraphicsEllipseItem
{
public:

/* Attributes */

    /* Appearance */
    QBrush brush;

    /* Physics */

    VectorXY * velocity;

    qreal mass;
    /*
     * Mass sign defines the nature of the accelerator:
     * Positive: Attracts.
     * Negative: Repels.
     *
     * The absolute value of the mass defines the intensity of
     * its gravity field by the inverse square law.
    */

    qreal radius;

/* Methods */

    /* Constructors */
    Accelerator();
    Accelerator(qreal radius, qreal mass, Qt::BrushStyle brush, Qt::GlobalColor color, qreal x, qreal y, qreal vx, qreal vy);

    /* Appearance */
    void paintAccelerator();    

    /* Destructor */
    ~Accelerator();
};

#endif // ACCELERATOR_H
