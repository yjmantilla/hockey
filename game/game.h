#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>
#include <time.h>

#include "puck.h"
#include "striker.h"
#include "wall.h"
#include "goal.h"
class Game : public QWidget
{
    Q_OBJECT

public:

    QGraphicsScene * scene;
    QGraphicsView * view;

    double width;
    double height;
    float timeStep=1;
    double boundary=100;

    Puck * puck;
    Striker * striker1;
    Striker * striker2;
    Goal * goal1;
    Goal * goal2;

    Wall * wallHU;
    Wall * wallHD;
    Wall * wallVL;
    Wall * wallVR;

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

    ~Game();

public slots:
    void animate();
};

#endif // GAME_H
