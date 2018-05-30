#ifndef GAME_H
#define GAME_H

#include <QtGlobal>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <QVector>
#include <QList>
#include <QtMath>
#include <QFile>

#include "puck.h"
#include "striker.h"
#include "wall.h"
#include "goal.h"
#include "field.h"
#include "accelerator.h"
#include "box.h"
#include "score.h"
#include "narrator.h"

class Game : public QWidget
{
    Q_OBJECT

public:

    QGraphicsScene * scene;
    QGraphicsView * view;

    qreal width;
    qreal height;
    qreal timeStep=0.1;
    qreal boundary=100;

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
    bool pause = true;

    bool goalAt1=false;
    bool goalAt2=false;

    /*A bot class may be created later to implement this*/

    bool bot1 = false;
    bool bot2 = true;
    qreal bot1Level = 1;/*Sets the difficulty of the bots given in multiples of the reaction time of humans, lower is harder*/
    qreal bot2Level = 1;
    bool bot1Dir = false; //false left, true right
    bool bot2Dir = true;
    QTimer * motionTimer;
    QTimer * boxTimer;
    QTimer * acceleratorTimer;
    QTimer * bot1Timer;
    QTimer * bot2Timer;


    /*A lot of the methods that involve other classes could be implemented in those classes, this might done later*/

    Game(QWidget *parent = 0, qreal width=900, qreal height=600, QString filename = "", bool load = false);
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
    int signRandomizer();
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
    void BoxesCollidingWithPuck();
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

};

#endif // GAME_H
