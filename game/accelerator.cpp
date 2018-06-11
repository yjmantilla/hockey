#include "accelerator.h"

Accelerator::Accelerator()
{
    /* Empty Constructor */
}

Accelerator::Accelerator(qreal radius, qreal mass, Qt::BrushStyle style, Qt::GlobalColor color, qreal x, qreal y,qreal vx, qreal vy)
{
    /* Set Physics Attributes */
    this->velocity = new VectorXY(vx,vy);
    this->radius=radius;
    this->mass = mass;

    /* Set Appearace Attributes */
    this->brush.setColor(color);
    this->brush.setStyle(style);
    this->setRect(x,y,2*radius,2*radius);
    this->setBrush(this->brush); //this is where it gets painted

}

void Accelerator::paintAccelerator()
{

    /*
     * This method paints the accelerator according to its nature.
     * Attracts: Paints it red.
     * Repels: Paints it blue.
     *
     * The nature is determined by the sign of the mass.
    */

    /* Configure Color */
    if (this->mass < 0)
    {
        this->brush.setColor(Qt::blue);
    }
    else
    {
        this->brush.setColor(Qt::red);
    }

    /* Execute the painting */
    this->setBrush(this->brush);

    return;
}

Accelerator::~Accelerator()
{
    /* Destructor */
    delete this->velocity;
}
