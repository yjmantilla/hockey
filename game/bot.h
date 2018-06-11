#ifndef BOT_H
#define BOT_H

/* Libraries */
#include <QtGlobal>
#include <QTimer>

/*
 * Bot Class
 *
 * This class represents the abstraction of a bot of the game.
 * This bot controls an assigned striker based on its base reaction time and its level.
 *
 * The base reaction time is measured in milliseconds.
 * The level is a multiplier of this base reaction time:
 * Level 1 means the bot reacts each "base reaction time" milliseconds.
 * Level 0.5 means it reacts each half of "base reaction time" milliseconds.
 *
 * So basically the lower the level, the harder the bot is. It reacts faster.
 *
*/
class Bot
{
public:

/* Attributes */

    bool state;
    /*
     * Boolean that defines whether the bot is activated (true) or not (false).
    */

    qreal baseReactionTime;

    qreal level;
    /*
     * Sets the difficulty of the bots given in multiples of the base reaction time
     * which is set in the game as the one of humans.
     * The lower the level the harder the bot is.
    */


    bool dir;
    /*
     * Boolean that tells the direction the bot wants to move:
     * False: Left.
     * True: Right.
    */


    QTimer * timer;
    /*
     * This timer controls the time cycle at which the bot reacts.
    */

/* Methods */

    /*Constructor*/
    Bot();
    Bot(bool state, qreal level, qreal baseReactionTime);

    /* Difficulty */
    void changeBotLevel(qreal level);

    /* Destructor */
    ~Bot();
};

#endif // BOT_H
