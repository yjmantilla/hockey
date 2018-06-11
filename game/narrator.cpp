#include "narrator.h"

#include <QDebug>

Narrator::Narrator(Qt::GlobalColor color)
{
    /*Set up Font*/
    this->setDefaultTextColor(color);
    this->setFont(QFont("times",16));

    /* Initial Text */
    this->setPlainText(QString("FIGHT!"));

}

void Narrator::narrate(QString string)
{
    /* This method just prints the string in the screen and in the application output */
    this->setPlainText(string);
    qDebug()<<string;
    return;
}
