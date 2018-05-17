#include "box.h"

Box::Box()
{
    this->velocity = new VectorXY(0,0);
}

QRectF Box::boundingRect() const
{
    return QRectF(0,0,40,40);
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pixmap;
    pixmap.load(":/box.png");
    painter->drawPixmap(this->boundingRect(),pixmap,pixmap.rect());
}

Box::~Box()
{
    delete this->velocity;
}
