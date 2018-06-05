#include "bot.h"

Bot::Bot()
{

}

Bot::Bot(bool state, qint32 level, quint32 baseReactionTime)
{
    this->state = state;
    this->level = level;
    this->timer = new QTimer();
    this->timer->start(baseReactionTime * this->level);

}

Bot::~Bot()
{
    delete this->timer;
}
