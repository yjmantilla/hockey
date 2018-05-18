#ifndef NARRATOR_H
#define NARRATOR_H
#include <QGraphicsTextItem>

class Narrator:public QGraphicsTextItem
{
public:
    Narrator(Qt::GlobalColor color);
    void narrate(QString string);


};

#endif // NARRATOR_H
