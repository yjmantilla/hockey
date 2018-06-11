#include "field.h"

Field::Field()
{
    /* Empty Constructor */
}

Field::Field(qreal viscosity)
{
    this->viscosity = viscosity;
}

void Field::setViscosity(qreal v)
{
    this->viscosity = v;
}

qreal Field::getViscosity()
{
    return this->viscosity;
}
