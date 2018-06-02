#ifndef WALL_H
#define WALL_H

#include <QGraphicsLineItem>
#include <QPen>

class Wall:public QGraphicsLineItem
{
public:

    qreal restitution;

    Wall();
    Wall(qreal x1, qreal y1, qreal x2, qreal y2, Qt::GlobalColor color);
    void setRestitution(qreal r);
    qreal getRestitution();
};

#endif // WALL_H
