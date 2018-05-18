#include "narrator.h"
#include <QFont>
#include <QDebug>

Narrator::Narrator(Qt::GlobalColor color)
{
    //draw the text
    this->setPlainText(QString("FIGHT!"));
    this->setDefaultTextColor(color);
    this->setFont(QFont("times",16));
}

void Narrator::narrate(QString string)
{
    this->setPlainText(string);
    qDebug()<<string;
    return;
}
