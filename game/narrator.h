#ifndef NARRATOR_H
#define NARRATOR_H

/* Libraries */
#include <QGraphicsTextItem>
#include <QFont>

/*
 * Narrator Class
 * This is an abstraction of a narrator of the game.
 * Basically it prints on the screen the events of the game.
*/
class Narrator:public QGraphicsTextItem
{
public:

/* Methods */
    /* Constructor */
    Narrator(Qt::GlobalColor color);

    /* Functionality*/
    void narrate(QString string);


};

#endif // NARRATOR_H
