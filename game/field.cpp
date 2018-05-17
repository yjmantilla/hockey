#include "field.h"

Field::Field()
{

}

Field::Field(qreal viscosity)
{
    this->viscosity = viscosity;
}

void Field::setViscosity(qreal v)
{
    this->viscosity = v;
}
