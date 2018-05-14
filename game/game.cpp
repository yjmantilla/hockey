#include "game.h"

Game::Game(QWidget *parent, qreal width, qreal height)
    : QWidget(parent)
{
    this->width = width;
    this->height = height;
    this->scene = new QGraphicsScene();
    this->view = new QGraphicsView();
    this->view->setFixedSize(this->width+100,this->height+100);
    this->scene->setSceneRect(0,0,this->width,this->height);
    this->view->setScene(this->scene);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    this->puck = new Puck(10,Qt::SolidPattern,Qt::yellow,this->scene->width()/2,this->scene->height()/2);
    this->striker1 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::red);
    this->striker2 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::blue);
    this->wallHU = new Wall(0,0,this->width,0);
    this->wallHD = new Wall(0,this->height,this->width,this->height);
    this->wallVL = new Wall(0,0,0,this->height);
    this->wallVR = new Wall(this->width,0,this->width,this->height);

    this->scene->addItem(this->puck);
    this->scene->addItem(this->striker1);
    this->scene->addItem(this->striker2);
    this->scene->addItem(this->wallHU);
    this->scene->addItem(this->wallHD);
    this->scene->addItem(this->wallVL);
    this->scene->addItem(this->wallVR);


    this->striker1->setPos(this->scene->width()/2,this->scene->height()-this->striker1->rect().height());
    this->striker2->setPos(this->scene->width()/2,0);

    this->moveR1=false;
    this->moveR2=false;
    this->moveL1=false;
    this->moveL2=false;

    this->motionTimer = new QTimer();
    connect(this->motionTimer,SIGNAL(timeout()),this,SLOT(animate()));
    motionTimer->start(10);

    this->view->setParent(this);
    this->view->show();

}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_J)
    {
        this->moveL1=true;
    }
    else if(event->key()==Qt::Key_A)
    {
        this->moveL2=true;
    }
    else if(event->key()==Qt::Key_D)
    {
        this->moveR2=true;
    }
    else if(event->key()==Qt::Key_L)
    {
        this->moveR1=true;
    }
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_J)
    {
        this->moveL1=false;
    }
    else if(event->key()==Qt::Key_A)
    {
        this->moveL2=false;
    }
    else if(event->key()==Qt::Key_D)
    {
        this->moveR2=false;
    }
    else if(event->key()==Qt::Key_L)
    {
        this->moveR1=false;
    }
}

Game::~Game()
{

}

void Game::animate()
{

    if(this->striker1->collidesWithItem(this->wallVL)){this->moveL1=false;}
    if(this->striker1->collidesWithItem(this->wallVR)){this->moveR1=false;}
    if(this->striker2->collidesWithItem(this->wallVL)){this->moveL2=false;}
    if(this->striker2->collidesWithItem(this->wallVR)){this->moveR2=false;}

    if(this->moveL1)
    {
        this->striker1->setPos(this->striker1->x()-this->striker1->xVelocity,this->striker1->y());
    }

    if(this->moveL2)
    {
        this->striker2->setPos(this->striker2->x()-this->striker2->xVelocity,this->striker2->y());
    }

    if(this->moveR1)
    {
        this->striker1->setPos(this->striker1->x()+this->striker1->xVelocity,this->striker1->y());
    }

    if(this->moveR2)
    {
        this->striker2->setPos(this->striker2->x()+this->striker2->xVelocity,this->striker2->y());
    }
}
