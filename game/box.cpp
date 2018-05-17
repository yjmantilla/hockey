#include "box.h"

Box::Box()
{

}

QRectF Box::boundingRect() const
{
    return QRectF(-40,-40,40,40);
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    pixmap.load(":/box.png");
    painter->drawPixmap(this->boundingRect(),pixmap,pixmap.rect());
}
