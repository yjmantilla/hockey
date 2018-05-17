#include "game.h"
#include <QDebug>
#include <QPointF>

Game::Game(QWidget *parent, qreal width, qreal height)
    : QWidget(parent)
{

    /*Set the Geometry of the Game*/

    this->width = width;
    this->height = height;
    this->scene = new QGraphicsScene();
    this->view = new QGraphicsView();
    this->view->setParent(this);
    this->view->setFixedSize(this->width+this->boundary,this->height+this->boundary);
    this->scene->setSceneRect(0,0,this->width,this->height);
    this->view->setScene(this->scene);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    /*Create Game Objects*/

    /*We want the puck with the same item coordinate origin as the scene to be able to track its position*/
    this->puck = new Puck(10,Qt::SolidPattern,Qt::yellow,0,0);
    this->striker1 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::red);
    this->striker2 = new Striker(0,0,this->width/8,this->height/60,Qt::SolidPattern,Qt::blue);
    this->wallHU = new Wall(0,0,this->width,0);
    this->wallHD = new Wall(0,this->height,this->width,this->height);
    this->wallVL = new Wall(0,0,0,this->height);
    this->wallVR = new Wall(this->width,0,this->width,this->height);
    this->goal1 = new Goal(this->width/2,this->height,this->width/5,Qt::white);
    this->goal2 = new Goal(this->width/2,0,this->width/5,Qt::white);
    this->field = new Field(0.001);




    /*Add Objects To Scene*/
    this->scene->addItem(this->puck);
    this->scene->addItem(this->striker1);
    this->scene->addItem(this->striker2);
    this->scene->addItem(this->wallHU);
    this->scene->addItem(this->wallHD);
    this->scene->addItem(this->wallVL);
    this->scene->addItem(this->wallVR);
    this->scene->addItem(this->goal1);
    this->scene->addItem(this->goal2);


    /*Center Puck*/
    this->centerPuck();
    this->velocifyPuck(1,3,3,11);

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

    return;
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

    return;
}

void Game::mousePressEvent(QMouseEvent *event)
{
    //Boxes Test
    /*
    this->Boxes.append(new Box());
    this->scene->addItem(this->Boxes.last());
    this->Boxes.last()->setPos(event->x(),event->y());
    */

    //Accelerators Test
    this->Accelerators.append(new Accelerator(10,-3000,Qt::SolidPattern,Qt::green,0,0,0,0));
    this->scene->addItem(this->Accelerators.last());
    this->Accelerators.last()->setPos(event->x(),event->y());

    return;
}

void Game::stopStrikersAtWallCollision()
{

    if(this->striker1->collidesWithItem(this->wallVL)){this->moveL1=false;}
    if(this->striker1->collidesWithItem(this->wallVR)){this->moveR1=false;}
    if(this->striker2->collidesWithItem(this->wallVL)){this->moveL2=false;}
    if(this->striker2->collidesWithItem(this->wallVR)){this->moveR2=false;}

    return;
}

void Game::moveStrikers()
{
    if(this->moveL1)
    {
        this->striker1->setPos(this->striker1->x()-this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveL2)
    {
        this->striker2->setPos(this->striker2->x()-this->striker2->velocity->getX(),this->striker2->y());
    }

    if(this->moveR1)
    {
        this->striker1->setPos(this->striker1->x()+this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveR2)
    {
        this->striker2->setPos(this->striker2->x()+this->striker2->velocity->getX(),this->striker2->y());
    }

    return;
}

void Game::bouncePuck()
{
    this->bouncePuckFromStrikers();

    if(this->puck->collidesWithItem(this->wallHD)&&!this->goalAt1){this->puck->velocity->setY(-1*this->puck->velocity->getY()*this->wallHD->restitution);}
    if(this->puck->collidesWithItem(this->wallHU)&&!this->goalAt2){this->puck->velocity->setY(-1*this->puck->velocity->getY()*this->wallHU->restitution);}
    if(this->puck->collidesWithItem(this->wallVL)){this->puck->velocity->setX(-1*this->puck->velocity->getX()*this->wallVL->restitution);}
    if(this->puck->collidesWithItem(this->wallVR)){this->puck->velocity->setX(-1*this->puck->velocity->getX()*this->wallVR->restitution);}
    return;
}

void Game::bouncePuckFromStrikers()
{
    if(this->puck->collidesWithItem(this->striker1)){this->puck->velocity->setY(-1*this->puck->velocity->getY()*this->striker1->restitution);}
    if(this->puck->collidesWithItem(this->striker2)){this->puck->velocity->setY(-1*this->puck->velocity->getY()*this->striker2->restitution);}
    return;
}

void Game::movePuck()
{
    this->updatePuckVelocity();
    this->updatePuckPosition();
    this->updatePuckAcceleration();
    qDebug() << "x:"<<this->puck->velocity->getX();
    qDebug() << "y:"<<this->puck->velocity->getY();
    if(this->didThePuckStop(0.5,0.5)){this->velocifyPuck(3,5,5,11);}
    //watchout , the ball gets suddenly impulsed

    return;

}

void Game::updatePuckPosition()
{
    this->puck->setX(this->puck->x()+this->puck->velocity->getX()*this->timeStep+0.5*this->puck->acceleration->getX()*this->timeStep*this->timeStep);
    this->puck->setY(this->puck->y()+this->puck->velocity->getY()*this->timeStep+0.5*this->puck->acceleration->getY()*this->timeStep*this->timeStep);
    return;
}

void Game::updatePuckVelocity()
{
    this->puck->velocity->setX(this->puck->velocity->getX()+this->puck->acceleration->getX()*this->timeStep);
    this->puck->velocity->setY(this->puck->velocity->getY()+this->puck->acceleration->getY()*this->timeStep);
    return;
}

void Game::scoreAtGoalCollision()
{
    if(this->puck->collidesWithItem(this->goal1)){qDebug()<<"goal1";this->goalAt1=true;}
    if(this->puck->collidesWithItem(this->goal2)){qDebug()<<"goal2";this->goalAt2=true;}
}

bool Game::isPuckOutside()
{
    /*
     * Notice this->puck->x/y are given in "item coordinates relative to the position they were initialized
     * (the origin of the scene)" So these coomparisons are relative to the origin of the scene
    */

    if(this->puck->y()<0 - this->boundary){return true;}
    if(this->puck->y()>this->height + this->boundary){return true;}
    if(this->puck->x()<0 - this->boundary){return true;}
    if(this->puck->x()>this->width + this->boundary){return true;}
    return false;
}

void Game::updatePuckAcceleration()
{
    /*Clear Previous acceleration to recalculate it*/
    this->puck->acceleration->setX(0);
    this->puck->acceleration->setY(0);

    /*For simplicity the formula is simplified as just a drag proportional to the velocity*/
    this->puck->acceleration->setX(-1*this->puck->velocity->getX()*this->field->viscosity);
    this->puck->acceleration->setY(-1*this->puck->velocity->getY()*this->field->viscosity);

    double dummyAcceleration;
    double dummyAngle;

    for(int i=0; i<this->Accelerators.size();i++)
    {
        dummyAcceleration = this->Accelerators.at(i)->mass / squaredDistanceToPuck(this->Accelerators.at(i)->x(),this->Accelerators.at(i)->y());
        dummyAngle = this->angleToPuck(this->Accelerators.at(i)->x(),this->Accelerators.at(i)->y());

        //qDebug()<<"magnitude:"<<dummyAcceleration;
        //qDebug()<<"angle:"<<dummyAngle;
        this->puck->acceleration->setX(this->puck->acceleration->getX() + dummyAcceleration*cos(dummyAngle));
        this->puck->acceleration->setY(this->puck->acceleration->getY() + dummyAcceleration*sin(dummyAngle));
    }

    //qDebug()<<"xA:"<<this->puck->acceleration->getX();
    //qDebug()<<"yA:"<<this->puck->acceleration->getY();
    return;

}

void Game::centerPuck()
{
    /*Center Puck , Notice items coordinates, Puck was initialized at the origin of the scene*/

    this->puck->setX(this->scene->width()/2);
    this->puck->setY(this->scene->height()/2);

    return;

}

void Game::markGoalAndRestart()
{
    if(this->isPuckOutside())
    {
        qDebug()<<"outside";
        this->centerPuck();
        this->velocifyPuck(1,3,3,11);
        /*Put here score register*/
        this->goalAt1=false;
        this->goalAt2=false;
    }

}

bool Game::didThePuckStop(qreal minX, qreal minY)
{
    if(qFabs(this->puck->velocity->getX()) < minX && qFabs(this->puck->velocity->getY()) < minY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Game::velocifyPuck(int minX, int maxX, int minY, int maxY)
{
    /*Give Puck Random Velocity*/

    /*Ensure x,y velocity is enough*/

    QRandomGenerator rand(time(NULL));

    this->puck->velocity->setY(rand.bounded(minY,maxY)*this->signRandomizer());

    //set low x velocity for it to be more frontal
    this->puck->velocity->setX(rand.bounded(minX,maxX)*this->signRandomizer());

    return;
}

double Game::squaredDistanceToPuck(qreal x, qreal y)
{
    /*Notice that both items need to have the same item origin*/
    return (((this->puck->x()-x)*(this->puck->x()-x))+((this->puck->y()-y)*(this->puck->y()-y)));
}

double Game::angleToPuck(qreal x, qreal y)
{
    return qAtan2(y - this->puck->y(),x-this->puck->x());
}

int Game::signRandomizer()
{
    QRandomGenerator rand(time(NULL));

    int dummy;

    while(1)
    {
        dummy = rand.bounded(-1,1);

        if(dummy != 0){break;}
    }

    return dummy;

}

Game::~Game()
{

}

void Game::animate()
{
    this->scoreAtGoalCollision();
    this->markGoalAndRestart();
    this->bouncePuck();
    this->movePuck();
    this->stopStrikersAtWallCollision();
    this->moveStrikers();

}
