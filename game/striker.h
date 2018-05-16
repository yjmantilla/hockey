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
    double xVelocity=3;

    double restitution=1.2;

    QBrush brush;

    Striker();
    Striker(qreal x, qreal y, qreal width, qreal height, Qt::BrushStyle style, Qt::GlobalColor color);
};

#endif // STRIKER_H
