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
#include <time.h>

#include "puck.h"
#include "striker.h"
#include "wall.h"
#include "goal.h"
#include "field.h"
#include "accelerator.h"
#include "box.h"


class Game : public QWidget
{
    Q_OBJECT

public:

    QGraphicsScene * scene;
    QGraphicsView * view;

    qreal width;
    qreal height;
    qreal timeStep=1;
    qreal boundary=100;

    Puck * puck;
    Striker * striker1;
    Striker * striker2;
    Goal * goal1;
    Goal * goal2;

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
    bool goalAt1=false;
    bool goalAt2=false;

    QTimer * motionTimer;
    QTimer * boxTimer;

    Game(QWidget *parent = 0, qreal width=900, qreal height=600);
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
    ~Game();

public slots:
    void animate();
    void addBox();
    void setPuckVisible();
    void restoreFieldViscosity();
    void restoreWallRestitution();
    void restoreStrikersRestitution();
};

#endif // GAME_H
