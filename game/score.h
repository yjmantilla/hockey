#ifndef SCORE_H
#define SCORE_H

#include <QGraphicsTextItem>
#include "striker.h"

class Score:public QGraphicsTextItem
{
public:
    Score(QGraphicsItem * = 0, Striker * striker = NULL);
    void increase();
    int getScore();
private:
    int score;
};


#endif // SCORE_H
