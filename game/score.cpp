#include "score.h"
#include <QFont>

Score::Score(QGraphicsItem * parent, Striker * striker):QGraphicsTextItem(parent)
{
    //initialize score to 0
    this->score =0;

    //draw the text

    this->setPlainText(/*QString("Score: ") + */QString::number(this->score));//Score: 0
    this->setDefaultTextColor(striker->brush.color());
    this->setFont(QFont("times",21));
    this->setPos(0,0);
}

void Score::increase()
{
    this->score++;
    //update indicator
    this->setPlainText(/*QString("Score: ") + */QString::number(this->score));
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
    this->setPlainText(/*QString("Score: ") + */QString::number(this->score));
    return;
}
