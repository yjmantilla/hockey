#ifndef GAME_H
#define GAME_H

#include <QtGlobal>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
//#include <QRandomGenerator>
#include <QVector>
#include <QList>
#include <QtMath>
#include <QFile>
#include <QTime>
#include <typeinfo>
#include <QMessageBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>

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


class Game : public QWidget
{
    Q_OBJECT

public:

    QGraphicsScene * scene;
    QGraphicsView * view;

    qreal width;
    qreal height;
    qreal timeStep;
    qreal boundary;
    qint32 maxScore;
    qint32 maxScoreStep;

    Puck * puck;
    Striker * striker1;
    Striker * striker2;
    Goal * goal1;
    Goal * goal2;
    Score * score1;
    Score * score2;
    Narrator * narrator;

    QVector<Accelerator *> accelerators;
    QVector<Box *> boxes;
    Wall * wallHU;
    Wall * wallHD;
    Wall * wallVL;
    Wall * wallVR;

    Field * field;

    bool moveL1;
    bool moveL2;
    bool moveR1;
    bool moveR2;
    bool pause;

    bool goalAt1;
    bool goalAt2;

    Bot * bot1;
    Bot * bot2;


    Controller * control1;
    Controller * control2;

    QTimer * motionTimer;
    QTimer * boxTimer;
    QTimer * acceleratorTimer;



    QMediaPlaylist * playlist;
    QMediaPlayer * musicPlayer;

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

    Game(QWidget *parent = 0, qreal width=900, qreal height=600, QString filename = "", bool load = false, bool bot1State=false, qreal bot1Level = 1, bool bot2State = true,qreal bot2Level=1, qint32 maxScore=21);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void stopStrikersAtWallCollision();
    void moveStrikers();
    void movePuck();
    void updatePuckPosition();
    void updatePuckVelocity();
    void scoreAtGoalCollision();
    bool isItemOutside(QGraphicsItem * item);
    void updatePuckAcceleration();
    void centerPuck();
    void markGoalAndRestart();
    bool didThePuckStop(qreal minX, qreal minY);
    double squaredDistanceToPuck(qreal x, qreal y);
    double angleToPuck(qreal x, qreal y);
    qint32 signRandomizer();
    qint32 getSign(qreal number);
    qreal boundedRandomizer(int min, int max);
    void velocify(VectorXY * velocity, int minX, int maxX, int minY, int maxY);
    void posify(QGraphicsItem * item, int minX, int maxX, int minY, int maxY);
    void moveEverything();
    void bounceEverything();
    void bounceFromWalls(QGraphicsItem * item, VectorXY *velocity);
    void bounceFromStrikers(QGraphicsItem * item, VectorXY *velocity);
    void moveItem(QGraphicsItem * item, VectorXY *velocity);    
    void deleteBox(Box *box);
    void deleteAccelerator(Accelerator * accel);
    void boxesCollidingWithPuck();
    void attractorsCollidingWithPuck();
    void chooseRandomEffect();
    void addAccelerator(qreal x, qreal y);
    void setPuckColor(Qt::GlobalColor color);
    void multiplyStrikerWidthOfRandomPlayer(qreal gain);
    qreal random10PercentMoreOrLess();
    void changeStrikerWidth(Striker * striker, Goal *goal, qreal gain);
    void multiplyGoalWidthOfRandomPlayer(qreal gain);
    void changeGoalWidth(Goal * goal, Striker *striker, qreal gain);
    int generateRandomPlayer();
    void randomStrikerVelocityForRandomPlayer();
    void negateRandomPlayerStrikerVelocity();
    qreal randomRestitutionForAllWalls();
    qreal randomRestitutionForAllPlayers();
    void botsify(Striker * striker, bool dir);
    bool whereIsTheDamnPuckAskedTheBot(Striker * striker);
    void saveGame(QString filename);
    void loadGame(QString filename);
    void readPort(QSerialPort * port, char * data, qint32 player);
    bool configurePort(QSerialPort * port, QString  portName);

    ~Game();

public slots:
    void animate();
    void addBox();
    void setPuckVisible();
    void restoreFieldViscosity();
    void restoreWallRestitution();
    void restoreStrikersRestitution();
    void addAccelerator(); //overload
    void reactBot1();
    void reactBot2();
    void readPorts();
    void readController1();
    void readController2();
    void newRound();
    void swapStrikers();
    void swapBackStrikers();

};

#endif // GAME_H
