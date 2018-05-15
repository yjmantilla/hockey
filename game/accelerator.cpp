#include "accelerator.h"

Accelerator::Accelerator()
{

}

Accelerator::Accelerator(double radius, double mass,Qt::BrushStyle style, Qt::GlobalColor color, double x, double y)
{
    this->radius=radius;
    this->mass = mass;
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush);
}
