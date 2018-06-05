#ifndef BOT_H
#define BOT_H
#include <QtGlobal>
#include <QTimer>
class Bot
{
public:

    bool state;
    /*Sets the difficulty of the bots given in multiples of the reaction time of humans, lower is harder*/
    qint32 level;

    bool dir; //false left, true right
    QTimer * timer;

    Bot();
    Bot(bool state, qint32 level, quint32  baseReactionTime);
    ~Bot();
};

#endif // BOT_H
