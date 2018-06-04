#ifndef PUCK_H
#define PUCK_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include "vectorxy.h"
#include <QMediaPlayer>

class Puck: public QGraphicsEllipseItem
{

public:
    qreal radius;

    QBrush brush;

    VectorXY * velocity;
    VectorXY * acceleration;

    QMediaPlayer * puckStrikerSound;
    QMediaPlayer * puckWallSound;

    Puck();
    Puck(qreal radius, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y);
    void setXVelocity(qreal xv);
    void setYVelocity(qreal yv);
    void setXAcceleration(qreal xa);
    void setYAcceleration(qreal ya);
    void setColor(Qt::GlobalColor color);
    ~Puck();

};


#endif // PUCK_H



