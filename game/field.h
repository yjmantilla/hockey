#ifndef FIELD_H
#define FIELD_H

#include <QtGlobal>

class Field
{
public:

    qreal viscosity;

    Field();
    Field(qreal viscosity);
    void setViscosity(qreal v);
};

#endif // FIELD_H
