#ifndef FIELD_H
#define FIELD_H

/* Libraries */
#include <QtGlobal>


/*
 * Field Class
 * Represents the field physical properties, mainly viscosity.
*/
class Field
{
public:
/* Attributes */
    qreal viscosity;

/* Methods */

    /* Constructor */
    Field();
    Field(qreal viscosity);

    /* Functionality */
    void setViscosity(qreal v);
    qreal getViscosity();
};

#endif // FIELD_H
