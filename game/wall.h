#ifndef WALL_H
#define WALL_H

/* Libraries */
#include <QGraphicsLineItem>
#include <QPen>

/*
 * Wall Class
 *
 * Abstraction of the limits of the field.
 * The main physical attributes are the dimensions and the restituion.
 * Its modeled as a line.
*/
class Wall:public QGraphicsLineItem
{
public:
/* Attributes */
    qreal restitution;

/* Methods */
    /* Constructor */
    Wall();
    Wall(qreal x1, qreal y1, qreal x2, qreal y2, Qt::GlobalColor color);

    /* Functionality */
    void setRestitution(qreal r);
    qreal getRestitution();
};

#endif // WALL_H
