#ifndef GOAL_H
#define GOAL_H

/* Libraries */
#include <QPen>
#include <QGraphicsLineItem>

/*
 * Goal Class
 *
 * Is the gap that let players score.
 * Its modeled with a line.
*/

class Goal:public QGraphicsLineItem
{
public:

/* Attributes */
    QPen pen;
    qreal width;

/* Methods */

    /* Constructor */
    Goal();
    Goal(qreal x1, qreal y1, qreal width,Qt::GlobalColor color);

    /* Functionality */
    qreal getWidth();
    void setWidth(qreal width);
};

#endif // GOAL_H
