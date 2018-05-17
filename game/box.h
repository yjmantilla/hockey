#ifndef BOX_H
#define BOX_H
#include <QGraphicsItem>
#include <QPainter>

class Box: public QGraphicsItem
{
public:
    Box();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // BOX_H
