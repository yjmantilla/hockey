#include "game.h"
#include <QDebug>

#define FIELD_VISCOSITY 0.001
#define FIELD_COLOR Qt::black
#define PUCK_COLOR Qt::green
#define MAX_D 30
#define MAX_V 30
#define MAX_A 50
#define MIN_XV_PUCK 3
#define MAX_XV_PUCK 5
#define MIN_YV_PUCK 3
#define MAX_YV_PUCK 11
#define MIN_XV_AandB 3
#define MAX_XV_AandB 7
#define MIN_YV_AandB 3
#define MAX_YV_AandB 7
#define PUCK_STOP_XV 0.5
#define PUCK_STOP_YV 0.5
#define BOX_SPAWN_TIME 5 //IN SECONDS
#define ACCELERATOR_SPAWN_TIME 3 //IN SECONDS
#define REFRESH_TIME 2 //IN MILLISECONDS
#define ACCELERATOR_RADIUS 20
#define ACCELERATOR_MASS 13000
#define MIN_GOAL_TO_STRIKER_RATIO 3
#define MIN_STRIKER_VELOCITY 7
#define MAX_STRIKER_VELOCITY 14
#define INVISIBLE_PUCK_TIME 3 //IN SECONDS
#define VISCOSITY_TIME 10
#define WALL_RESTITUTION_TIME 5 //IN SECONDS
#define STRIKER_RESTITUTION_TIME 5 //IN SECONDS
#define PUCK_RADIUS 15
#define WALL_COLOR Qt::yellow
#define STRIKER1_COLOR Qt::cyan
#define STRIKER2_COLOR Qt::magenta
#define NARRATOR_COLOR Qt::yellow
#define HUMAN_REACTION_TIME 250
#define BASE_RESTITUTION 1
#define NEW_GAME_GAIN 2
#define ACCELERATOR_PACK 10
#define PLAYER1_PORT "COM4"


Game::Game(QWidget *parent, qreal width, qreal height, QString filename, bool load, bool bot1State, qreal bot1Level, bool bot2State, qreal bot2Level, qint32 maxScore)
    : QWidget(parent)
{

    /*
     * Set seed ONCE
     * see https://stackoverflow.com/questions/2767383/use-of-qsrand-random-method-that-is-not-random
    */
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    this->timeStep = 0.1;
    this->boundary = 100;
    this->pause = true;
    this->goalAt1 = false;
    this->goalAt2 = false;


    /*Set the Geometry of the Game*/

    this->width = width;
    this->height = height;
    this->scene = new QGraphicsScene();
    this->view = new QGraphicsView();
    this->view->setParent(this);
    this->view->setFixedSize(this->width/*+this->boundary*/,this->height/*+this->boundary*/);
    this->width = this->width - this->boundary;
    this->height = this->height - this->boundary;
    this->scene->setSceneRect(0,0,this->width,this->height);
    this->view->setScene(this->scene);
    this->view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->view->setBackgroundBrush(QBrush(FIELD_COLOR, Qt::SolidPattern));

    /*Create Game Objects*/

    /*We want the puck with the same item coordinate origin as the scene to be able to track its position*/
    this->puck = new Puck(PUCK_RADIUS,Qt::SolidPattern,PUCK_COLOR,0,0);
    this->striker1 = new Striker(0,0,this->width/8,this->height/50,(MIN_STRIKER_VELOCITY + MAX_STRIKER_VELOCITY)/2,Qt::SolidPattern,STRIKER1_COLOR);
    this->striker2 = new Striker(0,0,this->width/8,this->height/50,(MIN_STRIKER_VELOCITY + MAX_STRIKER_VELOCITY)/2,Qt::SolidPattern,STRIKER2_COLOR);
    this->wallHU = new Wall(0,0,this->width,0,WALL_COLOR);
    this->wallHD = new Wall(0,this->height,this->width,this->height,WALL_COLOR);
    this->wallVL = new Wall(0,0,0,this->height,WALL_COLOR);
    this->wallVR = new Wall(this->width,0,this->width,this->height,WALL_COLOR);
    this->goal1 = new Goal(this->width/2,this->height,2*this->width/5,FIELD_COLOR);
    this->goal2 = new Goal(this->width/2,0,2*this->width/5,FIELD_COLOR);
    this->field = new Field(FIELD_VISCOSITY);
    this->score1 = new Score(0,this->striker1);
    this->score2 = new Score(0,this->striker2);
    this->narrator = new Narrator(NARRATOR_COLOR);
    this->maxScore = maxScore;
    this->maxScoreStep = maxScore;




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
    this->scene->addItem(this->score1);
    this->scene->addItem(this->score2);
    this->scene->addItem(this->narrator);


    /*Center Puck*/
    this->centerPuck();

    /*Give Puck Random Velocity*/
    /*Ensure x,y velocity is enough*/
    //set low x velocity for it to be more frontal
    this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);

    /*Set Striker,Scores Positions for each player*/
    this->striker1->setPos(this->scene->width()/2,this->scene->height()-this->striker1->rect().height());
    this->striker2->setPos(this->scene->width()/2,0);
    this->score1->setPos(this->scene->width()/2,this->scene->height()-this->striker1->rect().height()+25);
    this->score2->setPos(this->scene->width()/2,0-25);

    this->narrator->setPos(this->width/2,this->height/2);

    /*Initialize move booleans of players*/
    this->moveR1=false;
    this->moveR2=false;
    this->moveL1=false;
    this->moveL2=false;

    /*Animate Game with Timer*/
    this->motionTimer = new QTimer();
    connect(this->motionTimer,SIGNAL(timeout()),this,SLOT(animate()));

    /*Add Boxes and Accelerator*/
    this->boxTimer = new QTimer();
    connect(this->boxTimer,SIGNAL(timeout()),this,SLOT(addBox()));

    this->acceleratorTimer = new QTimer();
    connect(this->acceleratorTimer,SIGNAL(timeout()),this,SLOT(addAccelerator()));


    /*Show Game*/

    this->view->show();
    this->pause = true;

    /*Start Timers*/
    this->motionTimer->start(REFRESH_TIME);
    this->boxTimer->start(1000 * BOX_SPAWN_TIME);
    this->acceleratorTimer->start(1000 * ACCELERATOR_SPAWN_TIME);


    /*Load game if desired*/

    if(load)
    {
        this->loadGame(filename);
    }

    /*Set bots*/

    this->bot1 = new Bot(bot1State,bot1Level,HUMAN_REACTION_TIME);
    this->bot2 = new Bot(bot2State,bot2Level,HUMAN_REACTION_TIME);

    connect(this->bot1->timer,SIGNAL(timeout()),this,SLOT(reactBot1()));
    connect(this->bot2->timer,SIGNAL(timeout()),this,SLOT(reactBot2()));


    /*Controller*/

    this->control1 = new Controller();
    this->control2 = new Controller();

    /*Music*/
    this->playlist = new QMediaPlaylist();
    this->playlist->addMedia(QUrl("qrc:/music/GetLucky.mp3"));
    this->playlist->addMedia(QUrl("qrc:/music/TheNights.mp3"));
    this->playlist->addMedia(QUrl("qrc:/music/ABitOfDaftPunk.mp3"));
    this->playlist->addMedia(QUrl("qrc:/music/Aerodynamic.mp3"));
    this->playlist->addMedia(QUrl("qrc:/music/PentatonixDaftPunkMedley.mp3"));
    this->playlist->shuffle();
    this->playlist->setPlaybackMode(QMediaPlaylist::Random);
    this->musicPlayer = new QMediaPlayer();
    this->musicPlayer->setPlaylist(this->playlist);


    /*Sound Effects*/
    this->hitBox = new QMediaPlayer();
    this->hitBox->setMedia(QUrl("qrc:/soundEffects/hitBox.wav"));

    this->addAccel = new QMediaPlayer();
    this->addAccel->setMedia(QUrl("qrc:/soundEffects/addAccel.wav"));

    this->addAccels = new QMediaPlayer();
    this->addAccels->setMedia(QUrl("qrc:/soundEffects/addAccels.wav"));

    this->addBoxSound = new QMediaPlayer();
    this->addBoxSound->setMedia(QUrl("qrc:/soundEffects/addBox.wav"));

    this->effectEnds = new QMediaPlayer();
    this->effectEnds->setMedia(QUrl("qrc:/soundEffects/effectEnds.wav"));

    this->goalAt1Sound = new QMediaPlayer();
    this->goalAt1Sound->setMedia(QUrl("qrc:/soundEffects/goalAt1.wav"));

    this->goalAt2Sound = new QMediaPlayer();
    this->goalAt2Sound->setMedia(QUrl("qrc:/soundEffects/goalAt2.wav"));

    this->hitAccel = new QMediaPlayer();
    this->hitAccel->setMedia(QUrl("qrc:/soundEffects/hitAccel.wav"));


    this->hitWall = new QMediaPlayer();
    this->hitWall->setMedia(QUrl("qrc:/soundEffects/hitWall.wav"));

    this->hitStriker = new QMediaPlayer();
    this->hitStriker->setMedia(QUrl("qrc:/soundEffects/hitStriker.wav"));

    this->winSound = new QMediaPlayer();
    this->winSound->setMedia(QUrl("qrc:/soundEffects/wins.wav"));

    this->saveGame("reset");

    this->narrator->narrate("FIGHT!");

    this->musicPlayer->play();

}

void Game::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_J && !this->bot1->state )
    {
        this->moveL1=true;
    }
    else if(event->key()==Qt::Key_A && !this->bot2->state)
    {
        this->moveL2=true;
    }
    else if(event->key()==Qt::Key_D && !this->bot2->state )
    {
        this->moveR2=true;
    }
    else if(event->key()==Qt::Key_L && !this->bot1->state )
    {
        this->moveR1=true;
    }

    return;
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_J && !this->bot1->state)
    {
        this->moveL1 = false;
    }
    else if(event->key() == Qt::Key_A && !this->bot2->state)
    {
        this->moveL2 = false;
    }
    else if(event->key() == Qt::Key_D && !this->bot2->state)
    {
        this->moveR2 = false;
    }
    else if(event->key() == Qt::Key_L && !this->bot1->state )
    {
        this->moveR1 = false;
    }

    else if(event->key() == Qt::Key_P)
    {
        this->pause = !this->pause;
    }

    else if(event->key() == Qt::Key_F5)
    {
        this->saveGame("game.sav");
    }

    else if (event->key() == Qt::Key_F6)
    {
        this->loadGame("game.sav");
    }
    return;
}

void Game::mousePressEvent(QMouseEvent *event)
{
    /*This is for debug mainly*/

    //Boxes Test

    /*
    this->boxes.append(new Box());
    this->scene->addItem(this->boxes.last());
    qDebug()<<this->boxes.last()->scene();
    this->boxes.last()->setPos(event->x(),event->y());
    this->velocify(this->boxes.last()->velocity,1,2,2,3);
    */

    //Accelerators Test


    //this->addAccelerator(event->x(),event->y());


    /*Random Effect Test*/

    //this->chooseRandomEffect();

    /*Striker and Goal size Test*/
    //this->multiplyStrikerWidthOfRandomPlayer(this->randomDoubleOrHalfIt());qDebug()<<"Random Striker Width Changed: "<<this->striker1->rect().width()<<","<<this->striker2->rect().width();
    //this->multiplyStrikerWidthOfRandomPlayer(1.1);qDebug()<<"Random Striker Width Changed: "<<this->striker1->rect().width()<<","<<this->striker2->rect().width();
    //this->multiplyStrikerWidthOfRandomPlayer(0.9);qDebug()<<"Random Striker Width Changed: "<<this->striker1->rect().width()<<","<<this->striker2->rect().width();

    //qDebug()<<"g1: "<<this->goal1->x()<<","<<this->goal1->y()<<" OR "<<this->goal1->line().x1()<<","<<this->goal1->line().y1()<<" - "<<this->goal1->line().x2()<<","<<this->goal1->line().y2();
    //qDebug()<<"g2: "<<this->goal2->x()<<","<<this->goal2->y()<<" OR "<<this->goal2->line().x1()<<","<<this->goal2->line().y1()<<" - "<<this->goal2->line().x2()<<","<<this->goal2->line().y2();

    //this->multiplyGoalWidthOfRandomPlayer(0.9);

    //savegame test
    //this->saveGame("game.sav");
    //this->loadGame("game.sav");
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
        this->striker1->setPos(this->striker1->x()-this->timeStep*this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveL2)
    {
        this->striker2->setPos(this->striker2->x()-this->timeStep*this->striker2->velocity->getX(),this->striker2->y());
    }

    if(this->moveR1)
    {
        this->striker1->setPos(this->striker1->x()+this->timeStep*this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveR2)
    {
        this->striker2->setPos(this->striker2->x()+this->timeStep*this->striker2->velocity->getX(),this->striker2->y());
    }

    //qDebug()<<"1: "<<this->striker1->pos().x()<<","<<this->striker1->pos().y();
    //qDebug()<<"2: "<<this->striker2->pos().x()<<","<<this->striker2->pos().y();
    return;
}


void Game::movePuck()
{
    this->updatePuckPosition();

    this->updatePuckAcceleration();

    this->updatePuckVelocity();
    //qDebug() << "x:"<<this->puck->velocity->getX();
    //qDebug() << "y:"<<this->puck->velocity->getY();
    if(this->didThePuckStop(PUCK_STOP_XV,PUCK_STOP_YV)){this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);}
    //watchout , the ball gets suddenly impulsed

    return;

}

void Game::updatePuckPosition()
{    
    qreal dpx = this->puck->velocity->getX()*this->timeStep+0.5*this->puck->acceleration->getX()*this->timeStep*this->timeStep;
    qreal dpy = this->puck->velocity->getY()*this->timeStep+0.5*this->puck->acceleration->getY()*this->timeStep*this->timeStep;

    //if(qFabs(dpx) < MAX_D)
    {
        this->puck->setX(this->puck->x()+dpx);
    }

    //if(qFabs(dpy) < MAX_D)
    {
        this->puck->setY(this->puck->y()+dpy);
    }

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
    if(this->puck->collidesWithItem(this->goal1)){/*qDebug()<<"goal1"*/;this->goalAt1=true;}
    if(this->puck->collidesWithItem(this->goal2)){/*qDebug()<<"goal2"*/;this->goalAt2=true;}
}

bool Game::isItemOutside(QGraphicsItem * item)
{
    /*
     * Notice this->puck->x/y are given in "item coordinates relative to the position they were initialized
     * (the origin of the scene)" So these coomparisons are relative to the origin of the scene
    */

    if(item->y()<0 - 2*this->boundary){return true;}
    if(item->y()>this->height + 2*this->boundary){return true;}
    if(item->x()<0 - 2*this->boundary){return true;}
    if(item->x()>this->width + 2*this->boundary){return true;}
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

    for(int i=0; i<this->accelerators.size();i++)
    {
        dummyAcceleration = this->accelerators.at(i)->mass / squaredDistanceToPuck(this->accelerators.at(i)->x(),this->accelerators.at(i)->y());
        dummyAngle = this->angleToPuck(this->accelerators.at(i)->x(),this->accelerators.at(i)->y());

        //qDebug()<<"magnitude:"<<dummyAcceleration;
        //qDebug()<<"angle:"<<dummyAngle;
        this->puck->acceleration->setX(this->puck->acceleration->getX() + dummyAcceleration*cos(dummyAngle));
        this->puck->acceleration->setY(this->puck->acceleration->getY() + dummyAcceleration*sin(dummyAngle));
    }

    /*Note, if below is activated and also we deactivate the deletion of accelerators by touch then we can get planet-like accelerators that the puck will stick to, sadly the game is too easy with them*/
//    if(qFabs(this->puck->acceleration->getX()) > MAX_A)
//    {
//        this->puck->acceleration->setX(getSign(this->puck->acceleration->getX())*MAX_A);
//    }

//    if(qFabs(this->puck->acceleration->getY()) > MAX_A)
//    {
//        this->puck->acceleration->setY(getSign(this->puck->acceleration->getY())*MAX_A);
//    }

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
        //qDebug()<<"outside";
        this->centerPuck();
        this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);
        /*Put here score register*/
        /*We increase the score of the other player*/
        if(this->goalAt1){this->score2->increase();this->goalAt1Sound->play();this->changeGoalWidth(this->goal2,this->striker2,1.1);this->changeGoalWidth(this->goal1,this->striker1,0.9);}
        if(this->goalAt2){this->score1->increase();this->goalAt2Sound->play();this->changeGoalWidth(this->goal1,this->striker1,1.1);this->changeGoalWidth(this->goal2,this->striker2,0.9);}


        this->goalAt1=false;
        this->goalAt2=false;

        if(this->score1->getScore() == this->maxScore) //if the comparison is >= it will keep saying the temporal winner everytime one score is higher
        {
            this->winSound->play();
            this->narrator->narrate("PLAYER 1 WINS!");
            this->pause = true;
            //this->maxScore = this->maxScore * NEW_GAME_GAIN;//if they choose to continue playing
            this->maxScore = this->maxScore + this->maxScoreStep;

        }

        if(this->score2->getScore() >= this->maxScore)
        {
            this->winSound->play();
            this->narrator->narrate("PLAYER 2 WINS!");
            this->pause = true;
            //this->maxScore = this->maxScore * NEW_GAME_GAIN;
            this->maxScore = this->maxScore + this->maxScoreStep;
        }

        /*NOTE: A WIN BOOLEAN COULD BE INCLUDE SO THAT IS NOT POSSIBLE TO UNPAUSE THE GAME ONCE A PLAYER WINS*/
        /*RIGHT NOW THE PLAYERS CAN CONTINUE PLAYING IF THEY WISH*/
        /*IF THEY WISH TO CONTINUE PLAYING THE MAX SCORE CAN BE MULTIPLIED SO THE GAME CONTINUES*/
        /*OR JUST SUM IN maxScore STEPS*/
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

qint32 Game::signRandomizer()
{
    int dummy;

//    while(1)
//    {
//        //remember the upper bound is exclusive
//        dummy = QRandomGenerator::global()->bounded(-1,2);

//        if(dummy != 0){break;}
//    }


    // Random number between low and high
    dummy = qrand() % ((1 + 2) - 1) + 1;
    //qDebug()<<dummy;

    if(dummy == 2)
    {
        return 1;
    }

    else
    {
        return -1;
    }

        //return dummy;

}

qint32 Game::getSign(qreal number)
{
    if(number >= 0)
    {
        return 1;
    }

    else
    {
        return -1;
    }
}

qreal Game::boundedRandomizer(int min, int max)
{
    //return QRandomGenerator::global()->bounded(min,max + 1);

    return qrand() % ((max + 1) - min) + min;
}

void Game::velocify(VectorXY * velocity, int minX, int maxX, int minY, int maxY)
{

    velocity->setY(this->boundedRandomizer(minY,maxY)*this->signRandomizer());

    //set low x velocity for it to be more frontal
    velocity->setX(this->boundedRandomizer(minX,maxX)*this->signRandomizer());

    return;
}

void Game::posify(QGraphicsItem *item, int minX, int maxX, int minY, int maxY)
{
    /*This function may be improved by taking into account the bounding rect of the item so that it does not collide with the limits*/
    //item->setPos(this->boundedRandomizer(minX,maxX)*this->signRandomizer(),this->boundedRandomizer(minY,maxY)*this->signRandomizer());
    item->setPos(this->boundedRandomizer(minX,maxX),this->boundedRandomizer(minY,maxY));
    return;
}

void Game::moveEverything()
{
    this->movePuck();
    /* Everything else has constant velocity so we only need to update the position*/
    for(int i = 0; i < this->accelerators.size(); i++)
    {
        this->moveItem(this->accelerators.at(i),this->accelerators.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->accelerators.at(i)))
        {
            //this->scene->removeItem(this->accelerators.at(i));
            //delete this->accelerators.at(i);
            this->deleteAccelerator(this->accelerators.at(i));
            this->accelerators.removeAt(i);
            qDebug()<<"Accelerator Deleted";
        }
    }

    for(int i = 0; i < this->boxes.size(); i++)
    {
        this->moveItem(this->boxes.at(i),this->boxes.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->boxes.at(i)))
        {
            //this->scene->removeItem(this->Boxes.at(i));
            //delete this->Boxes.at(i);
            this->deleteBox(this->boxes.at(i));
            this->boxes.removeAt(i);
            qDebug()<<"Box Deleted";
        }
    }

    return;

}

/*Bounce Methods could be reimplemented so items wont get stuck on strikers or walls (they get inside walls and loop the cycle of negating the velocity)*/

void Game::bounceEverything()
{
    this->bounceFromStrikers(this->puck,this->puck->velocity);
    this->bounceFromWalls(this->puck,this->puck->velocity);

    for(int i = 0; i < this->accelerators.size(); i++)
    {
        this->bounceFromWalls(this->accelerators.at(i),this->accelerators.at(i)->velocity);
        this->bounceFromStrikers(this->accelerators.at(i),this->accelerators.at(i)->velocity);
    }

    for(int i = 0; i < this->boxes.size(); i++)
    {
        this->bounceFromWalls(this->boxes.at(i),this->boxes.at(i)->velocity);
        this->bounceFromStrikers(this->boxes.at(i),this->boxes.at(i)->velocity);
    }

    return;

}

void Game::bounceFromWalls(QGraphicsItem *item, VectorXY * velocity)
{
    bool dummy = false;

    if( item->collidesWithItem(this->wallHD) && !item->collidesWithItem(this->goal1) && velocity->getY() > 0)
    {
        velocity->setY(-1*velocity->getY()*this->wallHD->getRestitution());
        dummy = true;
    }

    if( item->collidesWithItem(this->wallHU) && !item->collidesWithItem(this->goal2) && velocity->getY() < 0)
    {
        velocity->setY(-1*velocity->getY()*this->wallHU->getRestitution());
        dummy = true;
    }

    if( item->collidesWithItem(this->wallVL) && velocity->getX() < 0 )
    {
        velocity->setX(-1*velocity->getX()*this->wallVL->getRestitution());
        dummy = true;
    }

    if( item->collidesWithItem(this->wallVR) && velocity->getX() > 0 )
    {
        velocity->setX(-1*velocity->getX()*this->wallVR->getRestitution());
        dummy = true;
    }

    if(dummy && item == this->puck)
    {
        this->puck->puckWallSound->play();
        this->hitWall->play();
    }

    return;
}

void Game::bounceFromStrikers(QGraphicsItem *item, VectorXY *velocity)
{
    if(item->collidesWithItem(this->striker1) && velocity->getY() > 0)
    {
        velocity->setY(-1*velocity->getY()*this->striker1->getRestitution());

        if(item == this->puck)
        {
            this->puck->puckStrikerSound->play();
            this->hitStriker->play();
        }

    }

    if(item->collidesWithItem(this->striker2) && velocity->getY() < 0)
    {
        velocity->setY(-1*velocity->getY()*this->striker2->getRestitution());

        if(item == this->puck)
        {
            this->puck->puckStrikerSound->play();
            this->hitStriker->play();
        }

    }


//    if(this->puck->puckStrikerSound->state() == QMediaPlayer::PlayingState)
//    {
//        this->puck->puckStrikerSound->setPosition(0);
//    }
//    else if (this->puck->puckStrikerSound->state() == QMediaPlayer::StoppedState)
//    {
//        this->puck->puckStrikerSound->play();
//    }




    return;

}

void Game::moveItem(QGraphicsItem *item, VectorXY *velocity)
{
    /*This assumes constant velocity*/

    item->setPos(item->x()+this->timeStep*velocity->getX(),item->y()+this->timeStep*velocity->getY());

    return;
}

void Game::deleteBox(Box * box)
{
    this->scene->removeItem(box);
    delete box;
    return;
}

void Game::deleteAccelerator(Accelerator *accel)
{
    this->scene->removeItem(accel);
    delete accel;
    return;
}

void Game::boxesCollidingWithPuck()
{
    for (int i = 0, n = this->puck->collidingItems().size();i < n;i++)
    {
        if(typeid (*(this->puck->collidingItems().at(i)))==typeid(Box))
        {
            this->hitBox->play();
            this->addBoxSound->play();
            this->boxes.removeOne((Box *)(this->puck->collidingItems().at(i)));
            this->deleteBox((Box *)(this->puck->collidingItems().at(i)));
            qDebug()<<"Box Deleted by Touch";
            this->chooseRandomEffect();
            return;
        }
    }
}

void Game::attractorsCollidingWithPuck()
{
    for (int i = 0, n = this->puck->collidingItems().size();i < n;i++)
    {
        if(typeid (*(this->puck->collidingItems().at(i)))==typeid(Accelerator))
        {
            this->hitAccel->play();
            this->addAccel->play();
            this->accelerators.removeOne((Accelerator *) (this->puck->collidingItems().at(i)));
            this->deleteAccelerator((Accelerator *) (this->puck->collidingItems().at(i)));
            qDebug()<<"Accelerator Deleted by Touch";
            return;
        }
    }
}

void Game::chooseRandomEffect()
{
    int effect = this->boundedRandomizer(1,10);
    qDebug()<<"Effect: "<<effect;
    QString comment;
    switch (effect)
    {
    case 1:
    {
        this->addAccels->play();

        for(qint32 i = 0; i < ACCELERATOR_PACK; i++)
        {
            this->addAccelerator(this->boundedRandomizer(0,this->width),this->boundedRandomizer(0,this->height));
        }

        comment = QString::number(ACCELERATOR_PACK) + QString(" Accelerators Added");
        this->narrator->narrate(comment);
        break;
    }
    case 2:
    {
        this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);
        comment = QString("Puck Velocified");
        this->narrator->narrate(comment);
        break;
    }
    case 3:
    {
        this->field->setViscosity(this->boundedRandomizer(1,100)*0.001);
        comment = QString("Field Viscosity Changed to: ") + QString::number(this->field->viscosity);
        this->narrator->narrate(comment);
        QTimer::singleShot(VISCOSITY_TIME*1000,this,SLOT(restoreFieldViscosity()));
        break;
    }
    case 4:
    {
        this->setPuckColor(FIELD_COLOR);//Qt::transparent? What if pucl is over somethinf else?
        comment = QString("Puck Invisible");
        this->narrator->narrate(comment);
        QTimer::singleShot(INVISIBLE_PUCK_TIME *1000,this,SLOT(setPuckVisible()));
        break;
    }
    case 5:
    {
        this->multiplyStrikerWidthOfRandomPlayer(this->random10PercentMoreOrLess());
        //qDebug()<<"Random Striker Width Changed: "<<this->striker1->rect().width()<<","<<this->striker2->rect().width();
        comment = QString("Random Striker Width Changed");
        this->narrator->narrate(comment);
        break;
    }
    case 6:
    {
        this->multiplyGoalWidthOfRandomPlayer(this->random10PercentMoreOrLess());
        comment = QString("Random Goal Width Changed");
        this->narrator->narrate(comment);
        break;
    }
    case 7:
    {
        this->randomStrikerVelocityForRandomPlayer();
        comment = QString("Random Striker Velocity For Random Player");
        this->narrator->narrate(comment);
        break;
    }
    case 8:
    {
        this->negateRandomPlayerStrikerVelocity();
        comment = QString("Negate Striker Velocity for Random Player");
        this->narrator->narrate(comment);
        break;
    }
    case 9:
    {
        qreal r = this->randomRestitutionForAllWalls();
        comment = QString("Walls with restitution of ") + QString::number(r)+QString(" for ")+QString::number(WALL_RESTITUTION_TIME)+QString(" seconds");
        this->narrator->narrate(comment);
        QTimer::singleShot(WALL_RESTITUTION_TIME*1000,this,SLOT(restoreWallRestitution()));
        break;
    }
    case 10:
    {
        qreal r = this->randomRestitutionForAllPlayers();
        comment = QString("Strikers with restitution of ") + QString::number(r)+QString(" for ") + QString::number(STRIKER_RESTITUTION_TIME) + QString(" seconds");
        this->narrator->narrate(comment);
        QTimer::singleShot(STRIKER_RESTITUTION_TIME*1000,this,SLOT(restoreStrikersRestitution()));
        break;
    }

    default:
        this->narrator->narrate("Why are you here?");
        break;
    }
}

void Game::addAccelerator(qreal x, qreal y)
{
    /*this is called by the boxes, ill let them only produce attractors*/
    this->accelerators.append(new Accelerator(ACCELERATOR_RADIUS,this->signRandomizer()*ACCELERATOR_MASS,Qt::SolidPattern,Qt::white,0,0,0,0));
    this->scene->addItem(this->accelerators.last());
    this->accelerators.last()->setPos(x,y);
    this->velocify(this->accelerators.last()->velocity,MIN_XV_AandB,MAX_XV_AandB,MIN_YV_AandB,MAX_YV_AandB);
    this->accelerators.last()->paintAccelerator();
    return;
}

void Game::setPuckColor(Qt::GlobalColor color)
{
        this->puck->setColor(color);
        qDebug()<<"Puck colored";
        return;
}

void Game::multiplyStrikerWidthOfRandomPlayer(qreal gain)
{
    int player = this->generateRandomPlayer();
    qDebug()<<"player: "<<player<<" gain: "<<gain<<" 1: "<<this->striker1->rect().width()<<" 2: "<<this->striker2->rect().width();
    if(player == 1){this->changeStrikerWidth(this->striker1,this->goal1,gain);}
    else if(player == 2){this->changeStrikerWidth(this->striker2,this->goal2,gain);}
    return;
}

qreal Game::random10PercentMoreOrLess()
{
    int dummy = this->boundedRandomizer(1,2);
    if(dummy == 1){qDebug()<<"gain: "<<0.9;return 0.9;}
    if(dummy == 2){qDebug()<<"gain: "<<1.1;return 1.1;}
    return 1;
}

void Game::changeStrikerWidth(Striker *striker, Goal * goal, qreal gain)
{
    if(gain > 1)
    {
        if (MIN_GOAL_TO_STRIKER_RATIO*striker->rect().width() < goal->getWidth()) //max is less 1.5 of the goal
        {
            striker->setWidth(striker->rect().width()*gain);
            return;
        }
        else
        {
            return;
        }
    }
    else if (gain < 1)
    {
        if (striker->rect().width() > this->width/32) //min is width/32
        {
            striker->setWidth(striker->rect().width()*gain);
        }
        else
        {
            return;
        }
    }

    return;
}

void Game::multiplyGoalWidthOfRandomPlayer(qreal gain)
{
    int player = this->generateRandomPlayer();
    qDebug()<<"player: "<<player<<" gain: "<<gain<<" 1: "<<this->goal1->width<<" 2: "<<this->goal2->width;
    if(player == 1){this->changeGoalWidth(this->goal1,this->striker1,gain);}
    else if(player == 2){this->changeGoalWidth(this->goal2,this->striker2,gain);}
    return;

}


void Game::changeGoalWidth(Goal * goal,Striker * striker, qreal gain)
{
    if(gain > 1)
    {
        if (goal->getWidth() < this->width) //max is width/2 (whole width)
        {
            goal->setWidth(goal->getWidth()*gain);
            return;
        }
        else
        {
            return;
        }
    }
    else if (gain < 1)
    {
        if (goal->getWidth() > MIN_GOAL_TO_STRIKER_RATIO * striker->rect().width()) //min is determined by striker width
        {
            goal->setWidth(goal->getWidth()*gain);
        }
        else
        {
            return;
        }
    }

    return;

}

int Game::generateRandomPlayer()
{
    return this->boundedRandomizer(1,2);
}

void Game::randomStrikerVelocityForRandomPlayer()
{
    int player = this->generateRandomPlayer();
    if(player==1){this->striker1->velocity->setX(this->boundedRandomizer(MIN_STRIKER_VELOCITY,MAX_STRIKER_VELOCITY));}
    if(player==2){this->striker2->velocity->setX(this->boundedRandomizer(MIN_STRIKER_VELOCITY,MAX_STRIKER_VELOCITY));}
    return;
}

void Game::negateRandomPlayerStrikerVelocity()
{
    int player = this->generateRandomPlayer();
    if(player==1){this->striker1->negateVelocity();}
    else
    if(player==2){this->striker2->negateVelocity();}
    return;
}

qreal Game::randomRestitutionForAllWalls()
{
    qreal r = this->boundedRandomizer(1,10)*0.1;
    this->wallHD->setRestitution(BASE_RESTITUTION+r);
    this->wallHU->setRestitution(BASE_RESTITUTION+r);
    this->wallVL->setRestitution(BASE_RESTITUTION+r);
    this->wallVR->setRestitution(BASE_RESTITUTION+r);
    return BASE_RESTITUTION+r;
}

qreal Game::randomRestitutionForAllPlayers()
{
    qreal r = this->boundedRandomizer(1,10)*0.1;
    this->striker1->setRestitution(BASE_RESTITUTION+r);
    this->striker2->setRestitution(BASE_RESTITUTION+r);
    return BASE_RESTITUTION+r;
}

void Game::botsify(Striker * striker, bool dir)
{
    if(puck->brush.color()!=FIELD_COLOR) //bots wont see when it is invisible as humans
    {
    if(!dir && !striker->collidesWithItem(this->wallVL)){striker->setX(striker->x()-this->timeStep*qFabs(striker->velocity->getX()));}
    if(dir && !striker->collidesWithItem(this->wallVR)){striker->setX(striker->x()+this->timeStep*qFabs(striker->velocity->getX()));}
    }
    return;

    /* A level of the bot can be done given the refresh rate of the bot every N samples, or de response time of the bot*/
}

bool Game::whereIsTheDamnPuckAskedTheBot(Striker *striker)
{
    if(puck->brush.color()!=FIELD_COLOR) //bots wont see when it is invisible as humans
    {
    if(striker->pos().x()+striker->rect().width()/2 > this->puck->pos().x()){return false;}//left
    if(striker->pos().x()+striker->rect().width()/2 < this->puck->pos().x()){return true;}//right
    }

    return false; //this shouldnt be reached ever though

    //also bots could only move within the goal width, but that just complicates the matter
}

void Game::saveGame(QString filename)
{
    this->pause = true;

    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "error saving!";
        QMessageBox::critical(this,tr("ERROR"),tr("Error saving game!"));
        return;
    }

    qDebug() << "saving data:";

    QTextStream out(&file);
    out << "GAME," << this->width << "," << this->height << "," << this->bot1->state << "," << this->bot1->level << "," << this->bot2->state << "," << this->bot2->level << "," << this->pause << ","<< this->maxScore<<","<<this->maxScoreStep<<"\n";
    out << "PUCK," << this->puck->x() << "," << this->puck->y() << "," << this->puck->velocity->getX() << "," << this->puck->velocity->getY() << "," << this->puck->acceleration->getX() << "," << this->puck->acceleration->getY() << "," << this->puck->radius << "\n";
    out << "STRIKER1," << this->striker1->x() << "," << this->striker1->y() << "," << this->striker1->velocity->getX() << "," << this->striker1->velocity->getY() << "," << this->striker1->rect().width() << "\n";
    out << "STRIKER2," << this->striker2->x() << "," << this->striker2->y() << "," << this->striker2->velocity->getX() << "," << this->striker2->velocity->getY() << "," << this->striker2->rect().width() << "\n";
    out << "GOAL1," << this->goal1->line().x1() << "," << this->goal1->line().y1() << "," << this->goal1->line().x2() << "," << this->goal1->width << "\n";
    out << "GOAL2," << this->goal2->line().x1() << "," << this->goal2->line().y1() << "," << this->goal2->line().x2() << "," << this->goal2->width << "\n";
    out << "SCORE1," << this->score1->getScore() << "\n";
    out << "SCORE2," << this->score2->getScore() << "\n";

    for (qint32 i = 0; i < this->accelerators.size(); i++)
    {
        out << "ACCEL," << this->accelerators.at(i)->x() << "," << this->accelerators.at(i)->y() << "," << this->accelerators.at(i)->velocity->getX() << "," << this->accelerators.at(i)->velocity->getY() << "," << this->accelerators.at(i)->mass << "," << this->accelerators.at(i)->radius << "\n";
    }

    for(qint32 i = 0; i < this->boxes.size(); i++)
    {
        out << "BOX," << this->boxes.at(i)->x() << "," << this->boxes.at(i)->y() << "," << this->boxes.at(i)->velocity->getX() << "," << this->boxes.at(i)->velocity->getY() << "\n";
    }

    qDebug() << "data saved successfully";
    this->narrator->narrate("GAME SAVED!");
    file.close();

    return;
}

void Game::loadGame(QString filename)
{
    this->pause = true;

    QFile file(filename);
        //qDebug() << "file opened:"<<file.open(QIODevice::ReadOnly);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<< "error loading!";
            QMessageBox::critical(this,tr("ERROR"),tr("Error loading game!"));
            return;
        }

        qDebug() << "loading data:";

        QString buffer;
        QList<QString> data;

        while(!file.atEnd())
        {
            buffer = file.readLine();
            if(buffer.length() > 3)
            {
                data.append(buffer);
                if(data.last().endsWith("\r\n"))
                {
                    data.last().remove("\r\n");
                }
                qDebug() << data.last();
            }

        }

        qDebug() << "data loaded successfully";

        qDebug() <<"cleaning game";

        while(!this->accelerators.isEmpty())
        {
            this->scene->removeItem(this->accelerators.last());
            this->accelerators.removeLast();
        }

        while(!this->boxes.isEmpty())
        {
            this->scene->removeItem(this->boxes.last());
            this->boxes.removeLast();
        }

        qDebug() << "loading data to game:";

        for (qint32 i = 0; i < data.size(); i++)
        {
            if(data.at(i).contains("GAME"))
            {
                qDebug() << "GAME:" << data.at(i).section(",",1);
                qreal width = data.at(i).section(",",1,1).toInt();
                qreal height = data.at(i).section(",",2,2).toInt();
                bool bot1State = data.at(i).section(",",3,3).toInt();
                qreal bot1Level = data.at(i).section(",",4,4).toFloat();
                bool bot2State = data.at(i).section(",",5,5).toInt();
                qreal bot2Level = data.at(i).section(",",6,6).toFloat();
                bool pause = data.at(i).section(",",7,7).toInt();
                qint32 maxScore = data.at(i).section(",",8,8).toInt();
                qint32 maxScoreStep = data.at(i).section(",",9,9).toInt();

                this->width = width;
                this->height = height;
                this->bot1->state = bot1State;
                this->bot1->level = bot1Level;
                this->bot2->state = bot2State;
                this->bot2->level = bot2Level;
                this->pause = pause;
                this->maxScore = maxScore;
                this->maxScoreStep = maxScoreStep;
            }

            else if(data.at(i).contains("PUCK"))
            {
                qDebug() << "PUCK:" << data.at(i).section(",",1);
                qreal px = data.at(i).section(",",1,1).toFloat();
                qreal py = data.at(i).section(",",2,2).toFloat();
                qreal vx = data.at(i).section(",",3,3).toFloat();
                qreal vy = data.at(i).section(",",4,4).toFloat();
                qreal ax = data.at(i).section(",",5,5).toFloat();
                qreal ay = data.at(i).section(",",6,6).toFloat();
                qreal rad = data.at(i).section(",",7,7).toFloat();

                this->puck->setPos(px,py);
                this->puck->velocity->setX(vx);
                this->puck->velocity->setY(vy);
                this->puck->acceleration->setX(ax);
                this->puck->acceleration->setY(ay);
                this->puck->radius = rad;

            }

           else if(data.at(i).contains("STRIKER1"))
           {
                qDebug() << "STRIKER1:" << data.at(i).section(",",1);
                qreal px = data.at(i).section(",",1,1).toFloat();
                qreal py = data.at(i).section(",",2,2).toFloat();
                qreal vx = data.at(i).section(",",3,3).toFloat();
                qreal vy = data.at(i).section(",",4,4).toFloat();
                qreal width = data.at(i).section(",",5,5).toFloat();

                this->striker1->setPos(px,py);
                this->striker1->velocity->setX(vx);
                this->striker1->velocity->setY(vy);
                //this->striker1->rect().setWidth(width);
                this->striker1->setWidth(width);
           }

           else if(data.at(i).contains("STRIKER2"))
           {
                qDebug() << "STRIKER2:" << data.at(i).section(",",1);
                qreal px = data.at(i).section(",",1,1).toFloat();
                qreal py = data.at(i).section(",",2,2).toFloat();
                qreal vx = data.at(i).section(",",3,3).toFloat();
                qreal vy = data.at(i).section(",",4,4).toFloat();
                qreal width = data.at(i).section(",",5,5).toFloat();

                this->striker2->setPos(px,py);
                this->striker2->velocity->setX(vx);
                this->striker2->velocity->setY(vy);
                //this->striker2->rect().setWidth(width);
                this->striker2->setWidth(width);
           }

           else if(data.at(i).contains("GOAL1"))
           {
                qDebug() << "GOAL1:" << data.at(i).section(",",1);
                qreal px1 = data.at(i).section(",",1,1).toFloat();
                qreal py1 = data.at(i).section(",",2,2).toFloat();
                qreal px2 = data.at(i).section(",",3,3).toFloat();
                qreal width = data.at(i).section(",",4,4).toFloat();

                this->goal1->setLine(px1 + this->goal1->getWidth()/2 - width/2,py1,px2 - this->goal1->getWidth()/2 + width/2,py1);
                this->goal1->width = width;
                this->goal1->setWidth(width);

                /*NOT SURE IF WIDTH LOADING FOR GOALS AND STRIKERS ARE CORRECT*/
           }

           else if(data.at(i).contains("GOAL2"))
           {
                qDebug() << "GOAL2:" << data.at(i).section(",",1);
                qreal px1 = data.at(i).section(",",1,1).toFloat();
                qreal py1 = data.at(i).section(",",2,2).toFloat();
                qreal px2 = data.at(i).section(",",3,3).toFloat();
                qreal width = data.at(i).section(",",4,4).toFloat();

                this->goal2->setLine(px1 + this->goal2->getWidth()/2 - width/2,py1,px2 - this->goal2->getWidth()/2 + width/2,py1);
                this->goal2->width = width;
                this->goal2->setWidth(width);
           }

           else if(data.at(i).contains("SCORE1"))
           {
                qDebug() << "SCORE1:" << data.at(i).section(",",1);
                qint32 score = data.at(i).section(",",1,1).toFloat();
                this->score1->setScore(score);
                this->score1->updateScoreText();
           }

           else if(data.at(i).contains("SCORE2"))
           {
                qDebug() << "SCORE2:" << data.at(i).section(",",1);
                qint32 score = data.at(i).section(",",1,1).toFloat();
                this->score2->setScore(score);
                this->score2->updateScoreText();
           }

           else if(data.at(i).contains("ACCEL"))
           {
                qDebug() << "ACCEL:" << data.at(i).section(",",1);
                qreal px = data.at(i).section(",",1,1).toFloat();
                qreal py = data.at(i).section(",",2,2).toFloat();
                qreal vx = data.at(i).section(",",3,3).toFloat();
                qreal vy = data.at(i).section(",",4,4).toFloat();
                qreal mass = data.at(i).section(",",5,5).toFloat();
                qreal rad = data.at(i).section(",",6,6).toFloat();

                this->accelerators.append(new Accelerator(rad,mass,Qt::SolidPattern,Qt::white,0,0,0,0));
                this->scene->addItem(this->accelerators.last());
                this->accelerators.last()->setPos(px,py);
                this->accelerators.last()->velocity->setVector(vx,vy);
                this->accelerators.last()->paintAccelerator();

           }

           else if(data.at(i).contains("BOX"))
           {
                 qDebug() << "BOX:" << data.at(i).section(",",1);
                 qreal px = data.at(i).section(",",1,1).toFloat();
                 qreal py = data.at(i).section(",",2,2).toFloat();
                 qreal vx = data.at(i).section(",",3,3).toFloat();
                 qreal vy = data.at(i).section(",",4,4).toFloat();

                 this->boxes.append(new Box());
                 this->scene->addItem(this->boxes.last());
                 this->boxes.last()->setPos(px,py);
                 this->boxes.last()->velocity->setVector(vx,vy);
           }

        }

        this->narrator->narrate("GAME LOADED!");
        qDebug() << "game loaded successfully";
}

bool Game::configurePort(QSerialPort *port, QString portName)
{
    port->setPortName(portName);

    if(port->open(QIODevice::ReadOnly))
    {
        if(!port->setBaudRate(QSerialPort::Baud9600))
        {
            qDebug()<<port->errorString();
            QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error setting baud rate"));
            return false;
        }

        if(!port->setDataBits(QSerialPort::Data8))
        {
            qDebug()<<port->errorString();
            QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error setting data bits"));
            return false;
        }

        if(!port->setParity(QSerialPort::NoParity))
        {
            qDebug()<<port->errorString();
            QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error setting parity"));
            return false;
        }

        if(!port->setStopBits(QSerialPort::OneStop))
        {
            qDebug()<<port->errorString();
            QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error setting stop bits"));
            return false;
        }

        if(!port->setFlowControl(QSerialPort::NoFlowControl))
        {
            qDebug()<<port->errorString();
            QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error setting flow control"));
            return false;
        }

        return true;
    }

    else
    {
        QMessageBox::critical(this,tr("ERROR_PORT"),tr("Error opening port!"));
        return false;
    }
}

void Game::readPort(QSerialPort *port, char *data, qint32 player)
{
    //if(port->bytesAvailable() > 1)
    {
        //port->clear();
        port->getChar(data);
        //qDebug() << data;
        QString data2(data);
        qDebug() << data2;
        //port->flush();
        //QEvent * sev;
        //port->event(sev);

        if(player == 1)
        {
            if(data2.contains("r"))
            {
                this->moveR1 = true;
                qDebug() << data2;
            }

            else if(data2.contains("l"))
            {
                this->moveL1 = true;
                qDebug() << data2;
            }

            else if (data2.contains("0"))
            {
                this->moveR1 = false;
                this->moveL1 = false;
                qDebug() << data2;
            }

            port->clear();
        }

        //return true;
    }

    //return false;
}

void Game::readPorts()
{
    //this->readPort(this->joyStick1,this->dataPort1,1);
    this->control1->read(&(this->moveR1),&(this->moveL1),this->bot1->state);
    this->control2->read(&(this->moveR2),&(this->moveL2),this->bot2->state);
    return;
}

void Game::readController1()
{
    this->control1->read(&(this->moveR1),&(this->moveL1),this->bot1->state);
    return;
}

void Game::readController2()
{
    this->control2->read(&(this->moveR2),&(this->moveL2),this->bot2->state);
    return;
}


Game::~Game()
{
    //delete a todos los apuntadores

    this->musicPlayer->stop();


    qDebug() <<"cleaning game";

    while(!this->accelerators.isEmpty())
    {
        this->scene->removeItem(this->accelerators.last());
        this->accelerators.removeLast();
    }

    while(!this->boxes.isEmpty())
    {
        this->scene->removeItem(this->boxes.last());
        this->boxes.removeLast();
    }

    delete this->puck;
    delete this->striker1;
    delete this->striker2;
    delete this->goal1;
    delete this->goal2;
    delete this->score1;
    delete this->score2;
    delete this->narrator;
    delete this->wallHD;
    delete this->wallHU;
    delete this->wallVL;
    delete this->wallVR;
    delete this->field;
    delete this->motionTimer;
    delete this->boxTimer;
    delete this->acceleratorTimer;
    delete this->bot1;
    delete this->bot2;
    delete this->control1;
    delete this->control2;
    delete this->musicPlayer;
    delete this->playlist;


}

void Game::animate()
{
    if(!this->pause)
    {
        //this->readPorts();
        if(this->bot1->state)
        {
            this->botsify(this->striker1,this->bot1->dir);
        }

        if(this->bot2->state)
        {
            this->botsify(this->striker2,this->bot2->dir);
        }

        this->boxesCollidingWithPuck();
        this->attractorsCollidingWithPuck();
        this->scoreAtGoalCollision();
        this->markGoalAndRestart();
        this->bounceEverything();

        this->moveEverything();
        this->stopStrikersAtWallCollision();
        this->moveStrikers();
    }

}

void Game::addBox()
{
    if(!this->pause)
    {
    this->boxes.append(new Box());
    this->scene->addItem(this->boxes.last());
    this->posify(this->boxes.last(),0+this->boxes.last()->boundingRect().width(),this->width-this->boxes.last()->boundingRect().width(),0+this->boxes.last()->boundingRect().height(),this->height-this->boxes.last()->boundingRect().height());
    this->velocify(this->boxes.last()->velocity,MIN_XV_AandB,MAX_XV_AandB,MIN_YV_AandB,MAX_YV_AandB);
    //this->addBoxSound->play();
    }
    return;

}

void Game::setPuckVisible()
{
    this->puck->setColor(PUCK_COLOR);
    this->effectEnds->play();
    this->narrator->narrate(QString("Puck Visible"));
    return;
}

void Game::restoreFieldViscosity()
{
    this->field->setViscosity(FIELD_VISCOSITY);
    this->effectEnds->play();
    this->narrator->narrate(QString("Field Viscosity Restored"));
    return;
}

void Game::restoreWallRestitution()
{
    this->wallHD->setRestitution(1);
    this->wallHU->setRestitution(1);
    this->wallVL->setRestitution(1);
    this->wallVR->setRestitution(1);
    this->effectEnds->play();
    this->narrator->narrate(QString("Walls Restitution Restored"));
    return;
}

void Game::restoreStrikersRestitution()
{
    this->striker1->setRestitution(1);
    this->striker2->setRestitution(1);
    this->effectEnds->play();
    this->narrator->narrate(QString("Strikers Restitution Restored"));
    return;
}

void Game::addAccelerator()
{
    //repulsors seem better for gameplay so i will keep spawn of repulsors constant add let boxes do the attractors--->deprecated
    if(!this->pause)
    {
        this->accelerators.append(new Accelerator(ACCELERATOR_RADIUS,this->signRandomizer()*ACCELERATOR_MASS,Qt::SolidPattern,Qt::white,0,0,0,0));
        this->scene->addItem(this->accelerators.last());
        this->posify(this->accelerators.last(),0,this->width,0,this->height);
        this->velocify(this->accelerators.last()->velocity,MIN_XV_AandB,MAX_XV_AandB,MIN_YV_AandB,MAX_YV_AandB);
        this->accelerators.last()->paintAccelerator();
        //this->addAccel->play();
    }


    return;
}

void Game::reactBot1()
{
    if(this->bot1->state)
    {
        this->bot1->dir = this->whereIsTheDamnPuckAskedTheBot(this->striker1);
    }
}

void Game::reactBot2()
{
    if(this->bot2->state)
    {
        this->bot2->dir = this->whereIsTheDamnPuckAskedTheBot(this->striker2);
    }
}
