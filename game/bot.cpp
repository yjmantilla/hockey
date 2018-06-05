#include "bot.h"

Bot::Bot()
{

}

Bot::Bot(bool state, qreal level, qreal baseReactionTime)
{
    this->state = state;
    this->level = level;
    this->baseReactionTime = baseReactionTime;
    this->timer = new QTimer();
    this->timer->start(this->baseReactionTime * this->level);

}

void Bot::changeBotLevel(qreal level)
{
    this->timer->stop();
    this->level = level;
    this->timer->start(this->baseReactionTime * this->level);
}

Bot::~Bot()
{
    delete this->timer;
}
