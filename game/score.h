#ifndef SCORE_H
#define SCORE_H

/* Libraries */
#include <QGraphicsTextItem>
#include <QFont>

/*
 * Score Class
 *
 * Is an abstracion of the score displayed on the screen.
 * Includes methods for its handling.
*/

class Score:public QGraphicsTextItem
{
public:

/* Methods */

    /*Constructor*/
    Score(QGraphicsItem * = 0, QColor color = Qt::blue);

    /* Functionality */
    void increase();
    int getScore();
    void setScore(qint32 score);
    void updateScoreText();

private:
/* Attributes */
    qint32 score;
};


#endif // SCORE_H
