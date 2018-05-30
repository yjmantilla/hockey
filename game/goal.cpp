#include "goal.h"
#include "game.h"

Goal::Goal()
{

}

Goal::Goal(qreal x1, qreal y1, qreal width, Qt::GlobalColor color)
{
    this->width = width;
    this->setLine(x1-this->width/2,y1,x1+this->width/2,y1);    
    this->setPen(QPen(color, 7, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin));
}

qreal Goal::getWidth()
{
    return this->width;
}

void Goal::setWidth(qreal width)
{
    this->setLine(this->line().x1() + this->getWidth()/2 - width/2,this->line().y1(),this->line().x2() - this->getWidth()/2 + width/2,this->line().y1());
    this->width = width;
    return;
}


