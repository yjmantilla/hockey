#ifndef PUCK_H
#define PUCK_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QObject>

class Puck: public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    double radius;
    double mass;

    QBrush brush;
    //QColor color;
    //Qt::BrushStyle style;

    double xVelocity;
    double yVelocity;
    double xAcceleration;
    double yAcceleration;


    Puck();
    Puck(double radius, Qt::BrushStyle brush, Qt::GlobalColor color, double x, double y);

public slots:
    //void move();


};


#endif // PUCK_H



