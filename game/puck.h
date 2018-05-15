#ifndef PUCK_H
#define PUCK_H

#include <QGraphicsEllipseItem>
#include <QBrush>

class Puck: public QObject,public QGraphicsEllipseItem
{

public:
    double radius;

    QBrush brush;

    double xVelocity=0;
    double yVelocity=0;
    double xAcceleration=0;
    double yAcceleration=0;


    Puck();
    Puck(double radius, Qt::BrushStyle brush, Qt::GlobalColor color, double x, double y);
    void setXVelocity(qreal xv);
    void setYVelocity(qreal yv);

};


#endif // PUCK_H



