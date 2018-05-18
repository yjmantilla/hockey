#include "box.h"

Box::Box()
{
    this->velocity = new VectorXY(0,0);
    this->setPos(0,0);
}

QRectF Box::boundingRect() const
{
    return QRectF(0,0,60,60);
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
