#include "score.h"

#define SCORE_SIZE 21

Score::Score(QGraphicsItem * parent, QColor color):QGraphicsTextItem(parent)
{

    /* Initialize score value */
    this->score =0;

    /* Configure Font */
    this->setFont(QFont("times",SCORE_SIZE));
    this->setPos(0,0);
    this->setDefaultTextColor(color);

    /* Set up Text */
    this->setPlainText(QString::number(this->score));

}

void Score::increase()
{
    /* Increase score and update Text */
    this->score++;
    this->setPlainText(QString::number(this->score));
}

int Score::getScore()
{
    return this->score;
}

void Score::setScore(qint32 score)
{
    this->score = score;
    return;
}

void Score::updateScoreText()
{
    /* Update the score Text once the Score attribute has been set */
    this->setPlainText(QString::number(this->score));
    return;
}
