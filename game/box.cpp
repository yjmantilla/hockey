#include "box.h"

#define BOX_SIDE 60
Box::Box()
{
    /* Set physics attributes */
    this->velocity = new VectorXY(0,0);

    /* Initialize the position of the box in the scene origin by default */
    this->setPos(0,0);
}

QRectF Box::boundingRect() const
{
    /* This is the bounding rect of the pixmap that will be loaded on it*/
    return QRectF(0,0,BOX_SIDE,BOX_SIDE);
}

void Box::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    /* Basically paints a rectangle with the image source*/

    QPixmap pixmap;
    pixmap.load(":/box.png");
    painter->drawPixmap(this->boundingRect(),pixmap,pixmap.rect());
}

Box::~Box()
{
    /* Destructor */
    delete this->velocity;
}
