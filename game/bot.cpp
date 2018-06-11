#include "bot.h"

Bot::Bot()
{
    /* Empty Constructor */
}

Bot::Bot(bool state, qreal level, qreal baseReactionTime)
{
    /*Set up attributes*/

    this->state = state;
    this->level = level;
    this->baseReactionTime = baseReactionTime;

    /* Set and start timer */

    this->timer = new QTimer();
    this->timer->start(this->baseReactionTime * this->level);

}

void Bot::changeBotLevel(qreal level)
{

    /* Stop timer (level affects it) */
    this->timer->stop();

    /* Change level and restart timer based on it */
    this->level = level;

    this->timer->start(this->baseReactionTime * this->level);
}

Bot::~Bot()
{
    /* Destructor */
    delete this->timer;
}
