#ifndef BOX_H
#define BOX_H
#include <QGraphicsItem>
#include <QPainter>
#include "vectorxy.h"

class Box: public QGraphicsItem
{
public:
    Box();

    VectorXY * velocity;

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    ~Box();
};

#endif // BOX_H
