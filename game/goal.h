#ifndef GOAL_H
#define GOAL_H
#include <QPen>
#include <QGraphicsLineItem>


class Goal:public QGraphicsLineItem
{
public:
    QPen pen;
    qreal width;

    Goal();
    Goal(qreal x1, qreal y1, qreal width,Qt::GlobalColor color);
    qreal getWidth();
    void setWidth(qreal width);
};

#endif // GOAL_H
