#include "puck.h"

Puck::Puck()
{

}

Puck::Puck(double radius, Qt::BrushStyle style, Qt::GlobalColor color, double x, double y)
{
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    //this->setRect(400,300,2*radius,2*radius);
    this->setBrush(this->brush);

}
