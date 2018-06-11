#ifndef PUCK_H
#define PUCK_H

/* Libraries */
#include <QGraphicsEllipseItem>
#include <QBrush>
#include "vectorxy.h"
#include <QMediaPlayer>


/*
 * Puck Class
 *
 * Abstraction of the puck (the disk hit by the strikers).
 *
*/
class Puck: public QGraphicsEllipseItem
{

public:
/* Attributes */

    /* Physics */
    qreal radius;        
    VectorXY * velocity;
    VectorXY * acceleration;

    /* Sound */
    QMediaPlayer * puckStrikerSound;
    QMediaPlayer * puckWallSound;

    /* Appearance */
    QBrush brush;

/* Methods */

    /* Constructor */
    Puck();
    Puck(qreal radius, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y);

    /* Appearance */
    void setColor(Qt::GlobalColor color);

    /* Destructor */
    ~Puck();

};


#endif // PUCK_H



