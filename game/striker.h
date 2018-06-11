#ifndef STRIKER_H
#define STRIKER_H

/*  Libraries */
#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QObject>
#include <QBrush>
#include "vectorxy.h"

/*
 * Striker Class
 *
 * This is the rectangle that the players control to hit the puck and other items in the game.
 *
 * It moves at a constant velocity that may be changed in game, and it only does so horizontally.
 * It also has a restitution coefficient that sets how much velocity is asserted upon the items
 * once they bounce from the striker.
*/

class Striker:public QGraphicsRectItem
{
public:

/* Attributes */

    /* Physics */
    VectorXY * velocity;
    /*
     * WARNING:
     * This striker velocity is set in an absolute value manner. Meaning it just the magnitude of the velocity.
     * The direction in which it moves is given by the sign applied to this magnitude in the cinematic position equation.
    */

    qreal restitution;

    /* Appearance */
    QBrush brush;

/* Methods */
    /* Constructor */
    Striker();
    Striker(qreal x, qreal y, qreal width, qreal height, qreal vel,Qt::BrushStyle style, Qt::GlobalColor color);

    /* Functionality */
    void setRestitution(qreal r);
    qreal getRestitution();
    void setWidth(qreal width);
    void negateVelocity();

    /* Destructor */
    ~Striker();
};

#endif // STRIKER_H
