#ifndef WALL_H
#define WALL_H

#include <QGraphicsLineItem>

class Wall:public QGraphicsLineItem
{
public:

    qreal restitution = 1;

    Wall();
    Wall(qreal x1, qreal y1, qreal x2, qreal y2);
};

#endif // WALL_H
