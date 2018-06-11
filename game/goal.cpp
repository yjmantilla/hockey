#include "goal.h"

Goal::Goal()
{
    /* Empty Constructor */
}

Goal::Goal(qreal x1, qreal y1, qreal width, Qt::GlobalColor color)
{

    /* Set up the line*/

    this->width = width;

    /*
     * Point x1 represents the middle of the line.
     * So we add and substract from this middle point half of the width to get the extreme points.
    */
    this->setLine(x1-this->width/2,y1,x1+this->width/2,y1);    

    /* Paint line */
    this->setPen(QPen(color, 7, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
}

qreal Goal::getWidth()
{
    return this->width;
}

void Goal::setWidth(qreal width)
{
    /*
     * Set up the width.
     * Because of the way the method setLine is implemented we first obtain the middle of the line:
     * that is the extreme +- half of the current width of the line depending of the side of the extreme
     * then we obtain the new extremes by adding +- the new width.
    */
    this->setLine(this->line().x1() + this->getWidth()/2 - width/2,this->line().y1(),this->line().x2() - this->getWidth()/2 + width/2,this->line().y1());

    /* Update the current width */
    this->width = width;
    return;
}


