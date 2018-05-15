#include "goal.h"

Goal::Goal()
{

}

Goal::Goal(qreal x1, qreal y1, double width, Qt::GlobalColor color)
{
    this->setLine(x1-width,y1,x1+width,y1);
    this->pen.setColor(color);
    this->setPen(pen);

}

