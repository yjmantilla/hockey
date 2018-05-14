#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>

#include "puck.h"
#include "striker.h"
#include "wall.h"

class Game : public QWidget
{
    Q_OBJECT

public:

    QGraphicsScene * scene;
    QGraphicsView * view;

    double width;
    double height;

    Puck * puck;
    Striker * striker1;
    Striker * striker2;
    Wall * wallHU;
    Wall * wallHD;
    Wall * wallVL;
    Wall * wallVR;

    bool moveL1;
    bool moveL2;
    bool moveR1;
    bool moveR2;
    bool pause;

    QTimer * motionTimer;

    Game(QWidget *parent = 0, qreal width=800, qreal height=600);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    ~Game();

public slots:
    void animate();
};

#endif // GAME_H
