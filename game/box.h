#ifndef BOX_H
#define BOX_H

/* Libraries */
#include <QGraphicsItem>
#include <QPainter>
#include "vectorxy.h"


/*
 * Box Class
 *
 * It represents the random box that which
 * upon contact with the puck activates an effect
 * an disappears.
 *
 * It moves with constant velocity around the field
 * unaffected by other bodies.
 *
*/

class Box: public QGraphicsItem
{
public:

/* Attributes */
    VectorXY * velocity;

/* Methods */
    /* Constructor */
    Box();

    /* Appearance */
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    /* Destructor */
    ~Box();
};

#endif // BOX_H
