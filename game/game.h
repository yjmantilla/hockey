#ifndef GAME_H
#define GAME_H

/* Qt Libraries*/
#include <QtGlobal>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <QVector>
#include <QList>
#include <QtMath>
#include <QFile>
#include <QTime>
#include <typeinfo>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
/* #include <QRandomGenerator> */

/* Personal Libraries */
#include "puck.h"
#include "striker.h"
#include "wall.h"
#include "goal.h"
#include "field.h"
#include "accelerator.h"
#include "box.h"
#include "score.h"
#include "narrator.h"
#include "bot.h"
#include "controller.h"

/*
 * Game Class
 *
 * This class unifies all of the game entities and
 * provides most of the game mechanics.
*/

class Game : public QWidget
{
    Q_OBJECT

public:

/* Attributes */

    /* Display */
    QGraphicsScene * scene;
    QGraphicsView * view;
    qreal width;
    qreal height;
    qreal boundary;

    /* Time */
    qreal timeStep;
    bool pause;
    QTimer * motionTimer;
    QTimer * boxTimer;
    QTimer * acceleratorTimer;

    /* Controllers */
    Controller * control1;
    Controller * control2;
    bool moveL1;
    bool moveL2;
    bool moveR1;
    bool moveR2;

    /* Bots */
    Bot * bot1;
    Bot * bot2;

    /* Game Objects */

        /* Scores */
        qint32 maxScore;
        qint32 maxScoreStep;
        Score * score1;
        Score * score2;

        /* Strikers */
        Striker * striker1;
        Striker * striker2;
        bool swappedStrikers;

        /* Goals */
        Goal * goal1;
        Goal * goal2;
        bool goalAt1;
        bool goalAt2;

        /* Field and Walls */
        Field * field;
        Wall * wallHU;
        Wall * wallHD;
        Wall * wallVL;
        Wall * wallVR;

        /* Items that move on Field */
        Puck * puck;
        QVector<Accelerator *> accelerators;
        QVector<Box *> boxes;


        /* Narrator */
        Narrator * narrator;

    /* Sound */
        /* Music */
        QMediaPlaylist * playlist;
        QMediaPlayer * musicPlayer;

        /*Effects*/
        QMediaPlayer * hitBox;
        QMediaPlayer * addAccel;
        QMediaPlayer * addAccels;
        QMediaPlayer * addBoxSound;
        QMediaPlayer * effectEnds;
        QMediaPlayer * goalAt1Sound;
        QMediaPlayer * goalAt2Sound;
        QMediaPlayer * hitAccel;
        QMediaPlayer * hitStriker;
        QMediaPlayer * hitWall;
        QMediaPlayer * winSound;

    /*A lot of the methods that involve other classes could be implemented in those classes, this might done later*/


/* Methods */

    /* Constructor */
    Game(QWidget *parent = 0, qreal width=900, qreal height=600, QString filename = "", bool load = false, bool bot1State=false, qreal bot1Level = 1, bool bot2State = true,qreal bot2Level=1, qint32 maxScore=21);

    /* Keyboard and Mouse Methods */
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

    /* Bot Methods */
    void botsify(Striker * striker, bool dir);
    bool whereIsTheDamnPuckAskedTheBot(Striker * striker);

    /* Puck Methods */
    void movePuck();
    void updatePuckPosition();
    void updatePuckVelocity();
    void updatePuckAcceleration();
    void centerPuck();
    bool didThePuckStop(qreal minX, qreal minY);
    qreal squaredDistanceToPuck(qreal x, qreal y);
    qreal angleToPuck(qreal x, qreal y);
        /* Puck Effects */
        void setPuckColor(Qt::GlobalColor color);

    /* Striker Methods */
    void stopStrikersAtWallCollision();
    void moveStrikers();
        /* Striker Effects */
        void multiplyStrikerWidthOfRandomPlayer(qreal gain);
        void changeStrikerWidth(Striker * striker, Goal *goal, qreal gain);
        void randomStrikerVelocityForRandomPlayer();
        qreal randomRestitutionForAllPlayers();
        void negateRandomPlayerStrikerVelocity();
        void swapStrikers();

    /* Goal and Score Methods */
    void scoreAtGoalCollision();
    void markGoalAndRestart();
        /* Goal Effects */
        void multiplyGoalWidthOfRandomPlayer(qreal gain);
        void changeGoalWidth(Goal * goal, Striker *striker, qreal gain);


    /* Wall Methods */
        /* Wall Effects */
        qreal randomRestitutionForAllWalls();

    /* Box Methods */
    void deleteBox(Box *box);
    void boxesCollidingWithPuck();
    void chooseRandomEffect();

    /* Accelerator Methods */
    void deleteAccelerator(Accelerator * accel);
    void attractorsCollidingWithPuck();
    void addAccelerator(qreal x, qreal y);


    /* General Item Methods */
    void velocify(VectorXY * velocity, int minX, int maxX, int minY, int maxY);
    void posify(QGraphicsItem * item, int minX, int maxX, int minY, int maxY);
    void moveEverything();
    void bounceEverything();
    void bounceFromWalls(QGraphicsItem * item, VectorXY *velocity);
    void bounceFromStrikers(QGraphicsItem * item, VectorXY *velocity);
    void moveItem(QGraphicsItem * item, VectorXY *velocity);
    bool isItemOutside(QGraphicsItem * item);

    /* General Utility Methods */
    qint32 signRandomizer();
    qint32 getSign(qreal number);
    qreal boundedRandomizer(int min, int max);
    qreal random10PercentMoreOrLess();
    int generateRandomPlayer();


    /* Game Save and Load Methods */
    void saveGame(QString filename);
    void loadGame(QString filename);


    /* Destructor */
    ~Game();

public slots:

    /* Animation Slots */
    void animate();
    void newRound();

    /* Bot Slots */
    void reactBot1();
    void reactBot2();

    /* Controller Slots */
    void readController1();
    void readController2();

    /* Add Boxes and Accelerators Slots */
    void addBox();
    void addAccelerator(); //This methods is overloaded

    /* Restore Effects Slots (for timed effects)*/
    void setPuckVisible();
    void restoreFieldViscosity();
    void restoreWallRestitution();
    void restoreStrikersRestitution();
    void swapBackStrikers();

};

#endif // GAME_H
