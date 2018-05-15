#include "game.h"

Game::Game(QWidget *parent, qreal width, qreal height)
    : QWidget(parent)
{

    /*Set the Geometry of the Game*/

    this->width = width;
    this->height = height;
    this->scene = new QGraphicsScene();
    this->view = new QGraphicsView();
    this->view->setParent(this);
    this->view->setFixedSize(this->width+100,this->height+100);
    this->scene->setSceneRect(0,0,this->width,this->height);
    this->view->setScene(this->scene);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    /*Create Game Objects*/

    this->puck = new Puck(10,Qt::SolidPattern,Qt::yellow,this->scene->width()/2,this->scene->height()/2);
    this->striker1 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::red);
    this->striker2 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::blue);
    this->wallHU = new Wall(0,0,this->width,0);
    this->wallHD = new Wall(0,this->height,this->width,this->height);
    this->wallVL = new Wall(0,0,0,this->height);
    this->wallVR = new Wall(this->width,0,this->width,this->height);

    /*Add Objects To Scene*/
    this->scene->addItem(this->puck);
    this->scene->addItem(this->striker1);
    this->scene->addItem(this->striker2);
    this->scene->addItem(this->wallHU);
    this->scene->addItem(this->wallHD);
    this->scene->addItem(this->wallVL);
    this->scene->addItem(this->wallVR);



    /*Give Puck Random Initial Velocity*/
    QRandomGenerator rand(time(NULL));
    this->puck->setYVelocity(rand.bounded(1,5));
    this->puck->setXVelocity(rand.bounded(-5,5));

    /*Set Striker Position for each player*/
    this->striker1->setPos(this->scene->width()/2,this->scene->height()-this->striker1->rect().height());
    this->striker2->setPos(this->scene->width()/2,0);

    /*Initialize move booleans of players*/
    this->moveR1=false;
    this->moveR2=false;
    this->moveL1=false;
    this->moveL2=false;

    /*Animate Game with Timer*/
    this->motionTimer = new QTimer();
    connect(this->motionTimer,SIGNAL(timeout()),this,SLOT(animate()));

    /*Show Game*/

    this->view->show();

    /*Start Timer*/
    motionTimer->start(10);

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

void Game::stopStrikersAtWallCollision()
{

    if(this->striker1->collidesWithItem(this->wallVL)){this->moveL1=false;}
    if(this->striker1->collidesWithItem(this->wallVR)){this->moveR1=false;}
    if(this->striker2->collidesWithItem(this->wallVL)){this->moveL2=false;}
    if(this->striker2->collidesWithItem(this->wallVR)){this->moveR2=false;}

}

void Game::moveStrikers()
{
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

void Game::bouncePuck()
{
    this->bouncePuckFromStrikers();

    if(this->puck->collidesWithItem(this->wallHD)){this->puck->setYVelocity(-1*this->puck->yVelocity*this->wallHD->restitution);}
    if(this->puck->collidesWithItem(this->wallHU)){this->puck->setYVelocity(-1*this->puck->yVelocity*this->wallHU->restitution);}
    if(this->puck->collidesWithItem(this->wallVL)){this->puck->setXVelocity(-1*this->puck->xVelocity*this->wallVL->restitution);}
    if(this->puck->collidesWithItem(this->wallVR)){this->puck->setXVelocity(-1*this->puck->xVelocity*this->wallVR->restitution);}

}

void Game::bouncePuckFromStrikers()
{
    if(this->puck->collidesWithItem(this->striker1)){this->puck->setYVelocity(-1*this->puck->yVelocity*this->striker1->restitution);}
    if(this->puck->collidesWithItem(this->striker2)){this->puck->setYVelocity(-1*this->puck->yVelocity*this->striker2->restitution);}

}

void Game::movePuck()
{
    this->updatePuckVelocity();
    this->updatePuckPosition();
}

void Game::updatePuckPosition()
{
    this->puck->setX(this->puck->x()+this->puck->xVelocity*this->timeStep+0.5*this->puck->xAcceleration*this->timeStep*this->timeStep);
    this->puck->setY(this->puck->y()+this->puck->yVelocity*this->timeStep+0.5*this->puck->yAcceleration*this->timeStep*this->timeStep);

}

void Game::updatePuckVelocity()
{
    this->puck->setXVelocity(this->puck->xVelocity+this->puck->xAcceleration*this->timeStep);
    this->puck->setYVelocity(this->puck->yVelocity+this->puck->yAcceleration*this->timeStep);
}

Game::~Game()
{

}

void Game::animate()
{
    this->bouncePuck();
    this->movePuck();

    this->stopStrikersAtWallCollision();
    this->moveStrikers();


}
