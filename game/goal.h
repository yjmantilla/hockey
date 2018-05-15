#ifndef GOAL_H
#define GOAL_H
#include <QPen>
#include <QGraphicsLineItem>


class Goal:public QGraphicsLineItem
{
public:
    QPen pen;

    Goal();
    Goal(qreal x1, qreal y1, double width,Qt::GlobalColor color);
};

#endif // GOAL_H
