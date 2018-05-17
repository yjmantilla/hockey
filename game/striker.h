#ifndef STRIKER_H
#define STRIKER_H

#include <QGraphicsRectItem>
#include <QKeyEvent>
#include <QObject>
#include <QBrush>
#include "vectorxy.h"



class Striker:public QObject,public QGraphicsRectItem
{
    Q_OBJECT
public:
    VectorXY * velocity;
    qreal restitution=1;

    QBrush brush;

    Striker();
    Striker(qreal x, qreal y, qreal width, qreal height, Qt::BrushStyle style, Qt::GlobalColor color);
    void setRestitution(qreal r);
    qreal getRestitution();
    ~Striker();
};

#endif // STRIKER_H
