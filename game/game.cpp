#include "game.h"
#include <QDebug>

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
    this->view->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    /*Create Game Objects*/

    /*We want the puck with the same item coordinate origin as the scene to be able to track its position*/
    this->puck = new Puck(10,Qt::SolidPattern,Qt::green,0,0);
    this->striker1 = new Striker(0,0,this->width/8,this->height/50,Qt::SolidPattern,Qt::cyan);
    this->striker2 = new Striker(0,0,this->width/8,this->height/50,Qt::SolidPattern,Qt::magenta);
    this->wallHU = new Wall(0,0,this->width,0,Qt::yellow);
    this->wallHD = new Wall(0,this->height,this->width,this->height,Qt::yellow);
    this->wallVL = new Wall(0,0,0,this->height,Qt::yellow);
    this->wallVR = new Wall(this->width,0,this->width,this->height,Qt::yellow);
    this->goal1 = new Goal(this->width/2,this->height,this->width/5,Qt::black);
    this->goal2 = new Goal(this->width/2,0,this->width/5,Qt::black);
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

    /*Give Puck Random Velocity*/
    /*Ensure x,y velocity is enough*/
    //set low x velocity for it to be more frontal
    this->velocify(this->puck->velocity,1,3,3,11);

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

    /*Add Boxes*/
    this->boxTimer = new QTimer();
    connect(this->boxTimer,SIGNAL(timeout()),this,SLOT(addBox()));
    /*Show Game*/

    this->view->show();

    /*Start Timers*/
    this->motionTimer->start(10);
    this->boxTimer->start(1000 * 15);


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


    this->Boxes.append(new Box());
    this->scene->addItem(this->Boxes.last());
    qDebug()<<this->Boxes.last()->scene();
    this->Boxes.last()->setPos(event->x(),event->y());
    this->velocify(this->Boxes.last()->velocity,1,2,2,3);


    //Accelerators Test

    this->Accelerators.append(new Accelerator(15,this->signRandomizer()*1500,Qt::SolidPattern,Qt::green,0,0,0,0));
    this->scene->addItem(this->Accelerators.last());
    this->Accelerators.last()->setPos(event->x(),event->y());
    this->velocify(this->Accelerators.last()->velocity,1,2,2,3);
    this->Accelerators.last()->paintAccelerator();

    return;
}

void Game::stopStrikersAtWallCollision()
{
    if(this->striker1->velocity->getX()>0)
    {
        if(this->striker1->collidesWithItem(this->wallVL)){this->moveL1=false;}
        if(this->striker1->collidesWithItem(this->wallVR)){this->moveR1=false;}
    }

    else
    {
        if(this->striker1->collidesWithItem(this->wallVL)){this->moveR1=false;}
        if(this->striker1->collidesWithItem(this->wallVR)){this->moveL1=false;}
    }

    if(this->striker2->velocity->getX()>0)
    {
        if(this->striker2->collidesWithItem(this->wallVL)){this->moveL2=false;}
        if(this->striker2->collidesWithItem(this->wallVR)){this->moveR2=false;}
    }

    else
    {
        if(this->striker2->collidesWithItem(this->wallVL)){this->moveR2=false;}
        if(this->striker2->collidesWithItem(this->wallVR)){this->moveL2=false;}
    }


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


void Game::movePuck()
{
    this->updatePuckVelocity();
    this->updatePuckPosition();
    this->updatePuckAcceleration();
    //qDebug() << "x:"<<this->puck->velocity->getX();
    //qDebug() << "y:"<<this->puck->velocity->getY();
    if(this->didThePuckStop(0.5,0.5)){this->velocify(this->puck->velocity,3,5,5,11);}
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

bool Game::isItemOutside(QGraphicsItem * item)
{
    /*
     * Notice this->puck->x/y are given in "item coordinates relative to the position they were initialized
     * (the origin of the scene)" So these coomparisons are relative to the origin of the scene
    */

    if(item->y()<0 - this->boundary){return true;}
    if(item->y()>this->height + this->boundary){return true;}
    if(item->x()<0 - this->boundary){return true;}
    if(item->x()>this->width + this->boundary){return true;}
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
    if(this->isItemOutside(this->puck))
    {
        qDebug()<<"outside";
        this->centerPuck();
        this->velocify(this->puck->velocity,1,3,3,11);
        /*Put here score register*/
        this->goalAt1=false;
        this->goalAt2=false;
    }

    return;
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
        //remember the upper bound is exclusive
        dummy = rand.bounded(-1,2);

        if(dummy != 0){break;}
    }

    return dummy;

}

void Game::velocify(VectorXY * velocity, int minX, int maxX, int minY, int maxY)
{
    QRandomGenerator rand(time(NULL));

    velocity->setY(rand.bounded(minY,maxY)*this->signRandomizer());

    //set low x velocity for it to be more frontal
    velocity->setX(rand.bounded(minX,maxX)*this->signRandomizer());

    return;
}

void Game::posify(QGraphicsItem *item, int minX, int maxX, int minY, int maxY)
{
    QRandomGenerator rand(time(NULL));

    item->setPos(rand.bounded(minX,maxX)*this->signRandomizer(),rand.bounded(minY,maxY)*this->signRandomizer());
    return;
}

void Game::moveEverything()
{
    this->movePuck();
    /* Everything else has constant velocity so we only need to update the position*/
    for(int i = 0; i < this->Accelerators.size(); i++)
    {
        this->moveItem(this->Accelerators.at(i),this->Accelerators.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->Accelerators.at(i)))
        {
            this->scene->removeItem(this->Accelerators.at(i));
            delete this->Accelerators.at(i);
            this->Accelerators.removeAt(i);
            qDebug()<<"Accelerator Deleted";
        }
    }

    for(int i = 0; i < this->Boxes.size(); i++)
    {
        this->moveItem(this->Boxes.at(i),this->Boxes.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->Boxes.at(i)))
        {
            this->scene->removeItem(this->Boxes.at(i));
            delete this->Boxes.at(i);
            this->Boxes.removeAt(i);
            qDebug()<<"Box Deleted";
        }
    }

    return;

}

void Game::bounceEverything()
{
    this->bounceFromStrikers(this->puck,this->puck->velocity);
    this->bounceFromWalls(this->puck,this->puck->velocity);

    for(int i = 0; i < this->Accelerators.size(); i++)
    {
        this->bounceFromWalls(this->Accelerators.at(i),this->Accelerators.at(i)->velocity);
        this->bounceFromStrikers(this->Accelerators.at(i),this->Accelerators.at(i)->velocity);
    }

    for(int i = 0; i < this->Boxes.size(); i++)
    {
        this->bounceFromWalls(this->Boxes.at(i),this->Boxes.at(i)->velocity);
        this->bounceFromStrikers(this->Boxes.at(i),this->Boxes.at(i)->velocity);
    }

    return;

}

void Game::bounceFromWalls(QGraphicsItem *item, VectorXY * velocity)
{
    if(item->collidesWithItem(this->wallHD)&&!item->collidesWithItem(this->goal1)){velocity->setY(-1*velocity->getY()*this->wallHD->getRestitution());}
    if(item->collidesWithItem(this->wallHU)&&!item->collidesWithItem(this->goal2)){velocity->setY(-1*velocity->getY()*this->wallHU->getRestitution());}
    if(item->collidesWithItem(this->wallVL)){velocity->setX(-1*velocity->getX()*this->wallVL->getRestitution());}
    if(item->collidesWithItem(this->wallVR)){velocity->setX(-1*velocity->getX()*this->wallVR->getRestitution());}
    return;
}

void Game::bounceFromStrikers(QGraphicsItem *item, VectorXY *velocity)
{
    if(item->collidesWithItem(this->striker1)){velocity->setY(-1*velocity->getY()*this->striker1->getRestitution());}
    if(item->collidesWithItem(this->striker2)){velocity->setY(-1*velocity->getY()*this->striker2->getRestitution());}
    return;

}

void Game::moveItem(QGraphicsItem *item, VectorXY *velocity)
{
    /*This assumes constant velocity*/

    item->setPos(item->x()+velocity->getX(),item->y()+velocity->getY());

    return;
}


Game::~Game()
{

}

void Game::animate()
{
    this->scoreAtGoalCollision();
    this->markGoalAndRestart();
    this->bounceEverything();
    this->moveEverything();
    this->stopStrikersAtWallCollision();
    this->moveStrikers();

}

void Game::addBox()
{

    this->Boxes.append(new Box());
    this->scene->addItem(this->Boxes.last());
    this->posify(this->Boxes.last(),0,this->width,0,this->height);
    this->velocify(this->Boxes.last()->velocity,1,2,2,3);
    return;

}
