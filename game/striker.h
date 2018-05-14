#ifndef STRIKER_H
#define STRIKER_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QObject>
#include <QBrush>



class Striker:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    double xVelocity=1;
    double yVelocity=0;

    double restitution;

    QBrush brush;
    //QColor color;
    //Qt::BrushStyle style;


    Striker();
    Striker(qreal x, qreal y, qreal width, qreal height, Qt::BrushStyle style, Qt::GlobalColor color);
};

#endif // STRIKER_H
