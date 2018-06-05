#ifndef BOT_H
#define BOT_H
#include <QtGlobal>
#include <QTimer>
class Bot
{
public:

    bool state;
    /*Sets the difficulty of the bots given in multiples of the reaction time of humans, lower is harder*/
    qreal level;
    qreal baseReactionTime;
    bool dir; //false left, true right
    QTimer * timer;

    Bot();
    Bot(bool state, qreal level, qreal baseReactionTime);
    void changeBotLevel(qreal level);
    ~Bot();
};

#endif // BOT_H
