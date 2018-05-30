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
    void setScore(qint32 score);
    void updateScoreText();
private:
    qint32 score;
};


#endif // SCORE_H
