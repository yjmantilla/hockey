#include "accelerator.h"

Accelerator::Accelerator()
{

}

Accelerator::Accelerator(qreal radius, qreal mass, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y,qreal vx, qreal vy)
{
    this->velocity = new VectorXY(vx,vy);
    this->radius=radius;
    this->mass = mass;
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush);
}

void Accelerator::paintAccelerator()
{
    if (this->mass < 0)
    {
        this->brush.setColor(Qt::blue);
    }
    else
    {
        this->brush.setColor(Qt::red);
    }

    this->setBrush(this->brush);

    return;
}

Accelerator::~Accelerator()
{
    delete this->velocity;
}
