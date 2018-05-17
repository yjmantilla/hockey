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

    QVector<Accelerator *> Accelerators;
    QVector<Box *> Boxes;
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

    Game(QWidget *parent = 0, qreal width=800, qreal height=600);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void stopStrikersAtWallCollision();
    void moveStrikers();
    void bouncePuck();
    void bouncePuckFromStrikers();
    void bouncePuckFromWalls();
    void movePuck();
    void updatePuckPosition();
    void updatePuckVelocity();
    void scoreAtGoalCollision();
    bool isPuckOutside();
    void updatePuckAcceleration();
    void centerPuck();
    void markGoalAndRestart();
    bool didThePuckStop(qreal minX, qreal minY);
    void velocifyPuck(int minX, int maxX, int minY, int maxY);
    double squaredDistanceToPuck(qreal x, qreal y);
    double angleToPuck(qreal x, qreal y);
    int signRandomizer();

    ~Game();

public slots:
    void animate();
};

#endif // GAME_H
