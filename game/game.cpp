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
#define BOX_SPAWN_TIME 11 //IN SECONDS
#define ACCELERATOR_SPAWN_TIME 7 //IN SECONDS
#define REFRESH_TIME 2 //IN MILLISECONDS
#define ACCELERATOR_RADIUS 20
#define ACCELERATOR_MASS 10000
#define MIN_GOAL_TO_STRIKER_RATIO 2
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
#define ACCELERATOR_PACK 17
#define PLAYER1_PORT "COM4"
#define NEW_ROUND_DELAY 2000
#define STRIKERS_SWAP_TIME 5 //IN SECONDS
#define EFFECT_VELOCITY_BONUS 11

Game::Game(QWidget *parent, qreal width, qreal height, QString filename, bool load, bool bot1State, qreal bot1Level, bool bot2State, qreal bot2Level, qint32 maxScore)
    : QWidget(parent)
{


    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());
    /*
     * This is for the random generators of the game
     * Set seed ONCE
     * see https://stackoverflow.com/questions/2767383/use-of-qsrand-random-method-that-is-not-random
    */


    /* Initialize properties of the game */
    this->timeStep = 0.1;
    this->boundary = 100;
    this->maxScore = maxScore;
    this->maxScoreStep = maxScore;
    this->swappedStrikers = false;

    /* Set the Geometry of the Game */

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

    /* Create Game Objects */

    /* We want the items with the same item coordinate origin as the scene to be able to track its position with the same reference */
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
    this->score1 = new Score(0,this->striker1->brush.color());
    this->score2 = new Score(0,this->striker2->brush.color());
    this->narrator = new Narrator(NARRATOR_COLOR);



    /* Add Objects To Scene */
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


    /* Center Puck */
    this->centerPuck();

    /* Set Positions */
    this->striker1->setPos(this->scene->width()/2 -this->striker1->rect().width()/2,this->scene->height()-this->striker1->rect().height());
    this->striker2->setPos(this->scene->width()/2 - this->striker2->rect().width()/2,0);
    this->score1->setPos(this->scene->width()/2,this->scene->height());
    this->score2->setPos(this->scene->width()/2,0 - this->striker2->rect().height() - 30);
    this->narrator->setPos(0,this->height);

    /* Initialize Booleans */
    this->moveR1=false;
    this->moveR2=false;
    this->moveL1=false;
    this->moveL2=false;
    this->pause = true;
    this->goalAt1 = false;
    this->goalAt2 = false;

    /* Animate Game with Timer */
    this->motionTimer = new QTimer();
    connect(this->motionTimer,SIGNAL(timeout()),this,SLOT(animate()));

    /* Add Boxes and Accelerator Timer */
    this->boxTimer = new QTimer();
    connect(this->boxTimer,SIGNAL(timeout()),this,SLOT(addBox()));

    this->acceleratorTimer = new QTimer();
    connect(this->acceleratorTimer,SIGNAL(timeout()),this,SLOT(addAccelerator()));


    /* Show Game */

    this->view->show();

    /* Load game if desired */

    if(load)
    {
        this->loadGame(filename);
    }

    /* Set bots */

    this->bot1 = new Bot(bot1State,bot1Level,HUMAN_REACTION_TIME);
    this->bot2 = new Bot(bot2State,bot2Level,HUMAN_REACTION_TIME);

    connect(this->bot1->timer,SIGNAL(timeout()),this,SLOT(reactBot1()));
    connect(this->bot2->timer,SIGNAL(timeout()),this,SLOT(reactBot2()));


    /* Allocate Controllers */

    this->control1 = new Controller();
    this->control2 = new Controller();

    /* Set Up Music */
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


    /* Set Up Sound Effects */
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

    /* Save reference game for reset */
    this->saveGame("reset");

    /* Start Music */
    this->musicPlayer->play();

    /* Start Timers */
    this->motionTimer->start(REFRESH_TIME);
    this->boxTimer->start(1000 * BOX_SPAWN_TIME);
    this->acceleratorTimer->start(1000 * ACCELERATOR_SPAWN_TIME);

    /* Ensure game starts from zero */
    this->loadGame("reset"); //without this the game wont start ok
    this->narrator->narrate("FIGHT!");

}


void Game::keyPressEvent(QKeyEvent *event)
{
    /*
     * This method detects a key pressed on the keyboard and acts accordingly
     * Basically:
     * The pair Key_J (Right) and Key_L (Left) move the first striker.
     * The pair Key_A (Right) and Key_D (Left) move the second striker.
     * If a striker has a bot activated on it then the movement is not done by the keys.     *
     *
     * Move Booleans:
     * These booleans save the direction in which the player wants to move.
     * The notation is move(direction: left=L , right=R)(player: 1,2).
     * So if the boolean is true, it moves in the corresponding direction for the assigned player.
     * This method acts on par with the keyReleaseEvent method, which sets the booleans false.
     * As a result the movement of the strikers responds in a "hold" mode, the players holds the key
     * while he wants to move in that direction.
     *
     * Game Shortcuts are provided too. See below.
    */


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

    /*
     * Shortcuts are also provided for key press, but these are on a click once basis.
     * Current shortcuts are:
     * Pause: in toggle mode.
     * Quick Save and Quick Load in a single shot mode, these are done always on the same file though: game.sav
    */

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

void Game::keyReleaseEvent(QKeyEvent *event)
{
    /*
     * Similar to keyPressEvent. This method detects a key release and acts accordingly.
     * It basically stop the movement in the direction once the player releases the key.
     *
     * Since the shortcuts are not on a "hold" mode they are not mentioned in this method.
    */
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

    return;
}

void Game::mousePressEvent(QMouseEvent *event)
{
    /*
     * This method is for debug mainly, the mouse is pressed
     * to test game functionalities.
    */

    //Boxes Test

    /*
    this->boxes.append(new Box());
    this->scene->addItem(this->boxes.last());
    qDebug()<<this->boxes.last()->scene();
    this->boxes.last()->setPos(event->x(),event->y());
    this->velocify(this->boxes.last()->velocity,1,2,2,3);
    */

    //Accelerators Test

//    qreal safeZone = this->boundedRandomizer(50,100);
//    for(qint32 i = 0; i < ACCELERATOR_PACK; i++)
//    {
//        this->addAccelerator(this->puck->x()+safeZone*this->signRandomizer(),this->puck->y()+safeZone*this->signRandomizer());
//    }


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

    //swap test
    //this->swapStrikers();
    return;
}

void Game::stopStrikersAtWallCollision()
{
    /*
     * This method checks the collision of both strikers to the vertical walls (left and right).
     * It impedes the striker to move in the corresponding direction if it collides with the associated wall.
     * It works by affecting directly the "Move Booleans"
     * The velocity in the strikers is assigned in an absolute value manner.
     * So we are checking the sign just to know if the striker is in the standard way.
     * Or if it is inverted by the negate striker velocity effect.
    */

    /* Striker 1 */

    if(this->striker1->velocity->getX()>0) //Moving in the standard direction.
    {
        if(this->striker1->collidesWithItem(this->wallVL))
        {
            this->moveL1=false;
        }

        if(this->striker1->collidesWithItem(this->wallVR))
        {
            this->moveR1=false;
        }
    }

    else //The Striker is inverted by the negateStrikerVelocity effect.
    {
        if(this->striker1->collidesWithItem(this->wallVL))
        {
            this->moveR1=false;
        }
        if(this->striker1->collidesWithItem(this->wallVR))
        {
            this->moveL1=false;
        }
    }

    /* Striker 2 */

    if(this->striker2->velocity->getX()>0) //Moving in the standard direction.
    {
        if(this->striker2->collidesWithItem(this->wallVL))
        {
            this->moveL2=false;
        }

        if(this->striker2->collidesWithItem(this->wallVR))
        {
            this->moveR2=false;
        }
    }

    else //The Striker is inverted by the negateStrikerVelocity effect.
    {
        if(this->striker2->collidesWithItem(this->wallVL))
        {
            this->moveR2=false;
        }

        if(this->striker2->collidesWithItem(this->wallVR))
        {
            this->moveL2=false;
        }
    }


    return;
}

void Game::moveStrikers()
{
    /*
     * This method just moves the striker using the "Move Booleans"
     * Notice that here is where the striker is set in the standart direction.
     * Since the striker velocity is interpreted as a magnitude and the direction is
     * given by the sign in the formula that precedes it (- left, + right),
     * then when the magnitude is negated the striker moves on the opposite direction.
     *
     * This method affect both strikers.
     *
     * It uses the cinematic formula to accomplish this:
     * r = ro +- vdt
    */

    if(this->moveL1) //move striker 1 to the left
    {
        this->striker1->setPos(this->striker1->x()-this->timeStep*this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveL2) //move striker 2 to the left
    {
        this->striker2->setPos(this->striker2->x()-this->timeStep*this->striker2->velocity->getX(),this->striker2->y());
    }

    if(this->moveR1) //move striker 1 to the right
    {
        this->striker1->setPos(this->striker1->x()+this->timeStep*this->striker1->velocity->getX(),this->striker1->y());
    }

    if(this->moveR2) //move striker 2 to the right
    {
        this->striker2->setPos(this->striker2->x()+this->timeStep*this->striker2->velocity->getX(),this->striker2->y());
    }

    return;
}


void Game::movePuck()
{
    /*
     * This method moves the puck within one time step.
     * We update position,velocity and acceleration in that order.
     * Position depends on current position, velocity and acceleration.
     * Velocity depends on current velocity and acceleration.
     * Acceleration depends on other objects only.
    */

    this->updatePuckPosition(); //takes into account current pos,vel,accel

    this->updatePuckVelocity(); //takes into account current vel,accel

    this->updatePuckAcceleration(); //takes into account other objects, it affects the next iteration of velocity and position



    if(this->didThePuckStop(PUCK_STOP_XV,PUCK_STOP_YV))
    {
        this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);
    }

    /*
     * This last "if" corresponds to the following:
     * Before accelerators where added in the game mechanics the puck would stop because of the viscosity
     * and its trajectory would be too predictable. Given this it was decided that once the puck stopped it
     * would acquire a sudden and random velocity vector so the game wouldnt be so predictable.
     *
     * Later as accelerators were implemented this wouldnt have been necessary but the feature was keep because it
     * provided a nice emergent gameplay when the viscosity was high in the field.
     * Basically the puck gets impulsed and stops in an unpredictable manner.
    */

    return;

}

void Game::updatePuckPosition()
{    
    /*
     * This method updates the position of the puck using the cinematic formula:
     * dr = ro + vdt + 0.5adt^2
     * This is calculted component-wise. And it is done in a timeStep manner, that is we calculated this step-wise
     * so the result is the displacement in a particular timeStep given the initial position it had.
    */

    qreal dpx = this->puck->velocity->getX()*this->timeStep+0.5*this->puck->acceleration->getX()*this->timeStep*this->timeStep;
    qreal dpy = this->puck->velocity->getY()*this->timeStep+0.5*this->puck->acceleration->getY()*this->timeStep*this->timeStep;

    this->puck->setX(this->puck->x()+dpx);
    this->puck->setY(this->puck->y()+dpy);

    return;
}

void Game::updatePuckVelocity()
{
    /*
     * This method updates the velocity of the puck using the cinematic formula:
     * dv = vo + adt
     * This is done both component-wise and timeStep-wise.
    */

    this->puck->velocity->setX(this->puck->velocity->getX()+this->puck->acceleration->getX()*this->timeStep);
    this->puck->velocity->setY(this->puck->velocity->getY()+this->puck->acceleration->getY()*this->timeStep);
    return;
}

void Game::scoreAtGoalCollision()
{

    /*
     * This method usea the "Goal Booleans" to inform the game that a goal is probably going to be made.
     * The goal is later marked when the Puck passes a boundary zone, that is, the goal is not marked upon touch by the puck.
     * But rather when it escapes the field by entering one of the goals.
    */

    if(this->puck->collidesWithItem(this->goal1))
    {
        /*qDebug()<<"goal1"*/;
        this->goalAt1=true;

    }

    if(this->puck->collidesWithItem(this->goal2))
    {
        /*qDebug()<<"goal2"*/;
        this->goalAt2=true;
    }
}

bool Game::isItemOutside(QGraphicsItem * item)
{
    /*
     * This method returns a boolean depending on the item being inside or outside of the field.
     * Notice item->x/y are given in "item coordinates relative to the position they were initialized
     * which is (the origin of the scene) by our design so these coomparisons are relative to the origin of the scene
     * It uses a boundary so that the item is way off the scene when it gets the quality of being outside of it (since the view is larger than the scene)
    */

    //up from the scene
    if(item->y()<0 - 2*this->boundary){return true;}

    //down from the scene
    if(item->y()>this->height + 2*this->boundary){return true;}

    //left from the scene
    if(item->x()<0 - 2*this->boundary){return true;}

    //right from the scene
    if(item->x()>this->width + 2*this->boundary){return true;}

    return false;
}

void Game::updatePuckAcceleration()
{
    /* Clear Previous acceleration to recalculate it, the acceleration is constant in a given timeStep */
    this->puck->acceleration->setX(0);
    this->puck->acceleration->setY(0);

    /* For simplicity and efficiency the formula of the viscosity is simplified as just a drag proportional to the velocity */
    this->puck->acceleration->setX(-1*this->puck->velocity->getX()*this->field->getViscosity());
    this->puck->acceleration->setY(-1*this->puck->velocity->getY()*this->field->getViscosity());


    /*
     * Now we add to the drag acceleration in each component the contribution of each accelerator present on the game
     * This is done for each accelerator using the formula:
     * a = m/r^2, and this is decomposed in its components using the sin and cosine functions given the angle of the accelerator to the puck.
     * We iterate this procedure over all accelerators adding all of them to each component of the acceleration of the puck.
    */

    qreal dummyAcceleration;
    qreal dummyAngle;

    for(int i=0; i<this->accelerators.size();i++)
    {
        dummyAcceleration = this->accelerators.at(i)->mass / squaredDistanceToPuck(this->accelerators.at(i)->x(),this->accelerators.at(i)->y());
        dummyAngle = this->angleToPuck(this->accelerators.at(i)->x(),this->accelerators.at(i)->y());

        this->puck->acceleration->setX(this->puck->acceleration->getX() + dummyAcceleration*cos(dummyAngle));
        this->puck->acceleration->setY(this->puck->acceleration->getY() + dummyAcceleration*sin(dummyAngle));
    }

    /*
     * Because the puck acquired high velocities when it approached attractors the next code was proposed.
     * Note, if the code is activated and also we deactivate the deletion of accelerators by touch then we can get planet-like accelerators behaviour:
     * the puck will stick to the attractors, sadly the game is too easy with this mechanic.
     * This code worked limiting the puck acceleration to a maximum value so it wouldnt acquire such high velocities.
     * This would be later be resolved in a better manner by deleting the accelerators once the puck touched them.
    */

//    if(qFabs(this->puck->acceleration->getX()) > MAX_A)
//    {
//        this->puck->acceleration->setX(getSign(this->puck->acceleration->getX())*MAX_A);
//    }

//    if(qFabs(this->puck->acceleration->getY()) > MAX_A)
//    {
//        this->puck->acceleration->setY(getSign(this->puck->acceleration->getY())*MAX_A);
//    }

    return;

}

void Game::centerPuck()
{
    /*
     * This method just centers the puck.
     * Notice that it uses items coordinates but the puck was initialized at the origin of the scene.
     * So this is in respect to the origin of the scene.s
    */

    this->puck->setX(this->scene->width()/2);
    this->puck->setY(this->scene->height()/2);

    return;

}

void Game::markGoalAndRestart()
{
    /*
     * This method marks a goal provided the puck is out of the field and it has done so by entering 1 of the goals of the players.
     *
     * Each time the goal is marked, the width of both player change:
     * scorer gets 10% more width
     * scored gets 10% less width
     * This is done so the game is balanced.
     *
     * Also a different sound is played for each goal and the narrator comments the event.
     *
     * When a player has won, the game pauses and provides the opportunity to continue playing by unpausing.
     * The winning score is updated in multiples of the original winning score.
     * For example: if the base winning score is 7, the next game will be of 14 and the next for 21 and so on.
    */

    /* Check the puck is outside, first requirement for a goal being made */
    if(this->isItemOutside(this->puck))
    {
        /*
         * When the item is outside, pause the game,
         * it will be later be resumed by activating a new round using the newRound slot.
        */
        this->pause = true;

        /*
         * Player 2 scores
         * implies the puck.y() is higher than the height of the scene
         * and it had touched player1's goal
         */

        if(this->goalAt1 && this->puck->y() > this->height )
        {
            this->score2->increase();
            this->goalAt1Sound->play();
            this->changeGoalWidth(this->goal2,this->striker2,1.1);
            this->changeGoalWidth(this->goal1,this->striker1,0.9);
            this->narrator->narrate("PLAYER 2 SCORES!");
        }

        /*
         * Player 1 scores
         * implies the puck.y() is lower than 0
         * and it had touched player2's goal
         */

        if(this->goalAt2 && this->puck->y() < 0){
            this->score1->increase();
            this->goalAt2Sound->play();
            this->changeGoalWidth(this->goal1,this->striker1,1.1);
            this->changeGoalWidth(this->goal2,this->striker2,0.9);
            this->narrator->narrate("PLAYER 1 SCORES!");
        }

        /* Reset the goal booleans once the goal has been marked */
        this->goalAt1=false;
        this->goalAt2=false;


        /* Check if one of the players has won */

        /* No player has won yet */
        if(!(this->score1->getScore() == this->maxScore) && !(this->score2->getScore() == this->maxScore))
        {
            /* Make a new round but not instantly but with a NEW_ROUND_DELAY */
            QTimer::singleShot(NEW_ROUND_DELAY,this,SLOT(newRound()));
        }


        /* Player 1 won */

        else if(this->score1->getScore() == this->maxScore)
        {
            this->winSound->play();
            this->narrator->narrate("PLAYER 1 WINS!");

            //if they choose to continue playing but the winning score is updated by using a gain
            //this->maxScore = this->maxScore * NEW_GAME_GAIN;

            //new game by multiples of the original winning score
            this->maxScore = this->maxScore + this->maxScoreStep;

            //We can make a new round automatically by uncommenting this
            //QTimer::singleShot(NEW_ROUND_DELAY,this,SLOT(newRound()));

        }

        /* Player 2 won */
        else if(this->score2->getScore() == this->maxScore)
        {
            this->winSound->play();
            this->narrator->narrate("PLAYER 2 WINS!");

            //if they choose to continue playing but the winning score is updated by using a gain
            //this->maxScore = this->maxScore * NEW_GAME_GAIN;

            //new game by multiples of the original winning score
            this->maxScore = this->maxScore + this->maxScoreStep;

            //We can make a new round automatically by uncommenting this
            //QTimer::singleShot(NEW_ROUND_DELAY,this,SLOT(newRound()));
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
    /*
     * This method returns true or false depending on the velocity of the puck in relation to a
     * minimum velocity we want it to have.
     *
     * If the absolute value of the velocity is lower in both components it returns true, else it returns false.
    */

    if(qFabs(this->puck->velocity->getX()) < minX && qFabs(this->puck->velocity->getY()) < minY)
    {
        return true;
    }
    else
    {
        return false;
    }
}

qreal Game::squaredDistanceToPuck(qreal x, qreal y)
{
    /*
     * This method returns the squared distance of the puck to an item position given by x,y parameters.
     * Notice that both items need to have the same item origin, which in the game is set as the origin of the scene.
     * The formula is: (puck.x()-item.x())^2 + (puck.y()-item.y())^2
     * This is used on the acceleration equation for gravitational attraction: m/r^2
    */
    return (((this->puck->x()-x)*(this->puck->x()-x))+((this->puck->y()-y)*(this->puck->y()-y)));
}

qreal Game::angleToPuck(qreal x, qreal y)
{
    /*
     * This method returns the angle of an item in respect to the puck.
     * That means that the puck acts as the x axis when calculating the angle.
     * qAtan2 is used because it provides the correct cuadrant already.
    */
    return qAtan2(y - this->puck->y(),x-this->puck->x());
}

qint32 Game::signRandomizer()
{
    /*
     * This method randomizes a sign.
     * Previously it was implemented with the QRandomGenerator class but this was deprecated because of compatibility issues.
     *
    */

    int dummy; //dummy that randomizes to either 1 or 2 (since we can only work with positive integers in qrand)
                //depending on the value acquired we will return either +1 or -1

    /* Previous implementation */
//    while(1)
//    {
//        //remember the upper bound is exclusive
//        dummy = QRandomGenerator::global()->bounded(-1,2);

//        if(dummy != 0){break;}
//    }

    /* Using qrand */

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

}

qint32 Game::getSign(qreal number)
{
    /*
     * This method just returns the sign of a number, is pretty straight forward.
    */
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
    /* This method returns a random number within the parameter limits (inclusively) */

    //previous implementation with QRandomGenerator
    //return QRandomGenerator::global()->bounded(min,max + 1);

    //implementation using qrand
    return qrand() % ((max + 1) - min) + min;
}

void Game::velocify(VectorXY * velocity, int minX, int maxX, int minY, int maxY)
{
    /*
     * This method gives a random velocity to a velocity vector (which is an VectorXY actually and is from an item)
     * The other parameters define the random generator range.
     * if you want the item to be more "frontal" give it high y velocities and low x velocities
    */

    velocity->setY(this->boundedRandomizer(minY,maxY)*this->signRandomizer());
    velocity->setX(this->boundedRandomizer(minX,maxX)*this->signRandomizer());

    return;
}

void Game::posify(QGraphicsItem *item, int minX, int maxX, int minY, int maxY)
{
    /*
     * This method gives a random position to an graphics item within the range set by the parameters.
     * This function may be improved by taking into account the bounding rect of the item so that it does not collide with the limits
     * The positions are in respect to the origin of the scene
    */

    item->setPos(this->boundedRandomizer(minX,maxX),this->boundedRandomizer(minY,maxY));
    return;
}

void Game::moveEverything()
{
    /* First we move the puck which has the most complex cinematics */
    this->movePuck();

    /* Everything else has constant velocity so we only need to update the position (no acceleration involved) */

    /* Iterate over accelerators */
    for(int i = 0; i < this->accelerators.size(); i++)
    {
        this->moveItem(this->accelerators.at(i),this->accelerators.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->accelerators.at(i)))
        {
            this->deleteAccelerator(this->accelerators.at(i));
            this->accelerators.removeAt(i);
            qDebug()<<"Accelerator Deleted";
        }
    }

    /* Iterate over boxes */
    for(int i = 0; i < this->boxes.size(); i++)
    {
        this->moveItem(this->boxes.at(i),this->boxes.at(i)->velocity);

        /*Delete Item if it is outside*/
        if(this->isItemOutside(this->boxes.at(i)))
        {
            this->deleteBox(this->boxes.at(i));
            this->boxes.removeAt(i);
            qDebug()<<"Box Deleted";
        }
    }

    return;

}


void Game::bounceEverything()
{
    /*
     * This method bounce everything from the walls and from the strikers.
     * It does so for the puck, the accelerators and the boxes.
    */

    //puck
    this->bounceFromStrikers(this->puck,this->puck->velocity);
    this->bounceFromWalls(this->puck,this->puck->velocity);

    //accelerators
    for(int i = 0; i < this->accelerators.size(); i++)
    {
        this->bounceFromWalls(this->accelerators.at(i),this->accelerators.at(i)->velocity);
        this->bounceFromStrikers(this->accelerators.at(i),this->accelerators.at(i)->velocity);
    }

    //boxes
    for(int i = 0; i < this->boxes.size(); i++)
    {
        this->bounceFromWalls(this->boxes.at(i),this->boxes.at(i)->velocity);
        this->bounceFromStrikers(this->boxes.at(i),this->boxes.at(i)->velocity);
    }

    return;

}

void Game::bounceFromWalls(QGraphicsItem *item, VectorXY * velocity)
{
    /*
     * This method bounces a graphic item from the wall and it affects its associated velocity vector passed down as a parameter by reference.
     * It takes into account the restitution of the walls.
     * It was reimplemented so items wouldnt get stuck on walls (they get inside walls and loop the cycle of negating the velocity)
     * This was achieved by only inverting the velocity when it hit the wall by moving towards them and from the inside of the field.
     * That is, items are free to enter the field if they are outside but if they are on the inside they will bounce off the walls.
     * This was implemented by taking into account the sign of the velocity we want to negate upon bouncing.
    */

    bool dummy = false; //this boolean just checks if a bounce was made

    // horizontal down wall, bounce only if the item is moving down (positive y velocity) and it doesnt collide with the goal 1
    if( item->collidesWithItem(this->wallHD) && !item->collidesWithItem(this->goal1) && velocity->getY() > 0)
    {
        velocity->setY(-1*velocity->getY()*this->wallHD->getRestitution());
        dummy = true;
    }

    // horizontal up wall, bounce only if the item is moving up (negative y velocity) and it doesnt collide with the goal 2
    if( item->collidesWithItem(this->wallHU) && !item->collidesWithItem(this->goal2) && velocity->getY() < 0)
    {
        velocity->setY(-1*velocity->getY()*this->wallHU->getRestitution());
        dummy = true;
    }

    // vertical left wall, bounce only if the item is moving left (negative x velocity)
    if( item->collidesWithItem(this->wallVL) && velocity->getX() < 0 )
    {
        velocity->setX(-1*velocity->getX()*this->wallVL->getRestitution());
        dummy = true;
    }

    // vertical right wall, bounce only if the item is moving right (positive x velocity)
    if( item->collidesWithItem(this->wallVR) && velocity->getX() > 0 )
    {
        velocity->setX(-1*velocity->getX()*this->wallVR->getRestitution());
        dummy = true;
    }

    //if the item that bounced was the puck, play a sound
    if(dummy && item == this->puck)
    {
        this->puck->puckWallSound->play();
        this->hitWall->play();
    }

    return;
}

void Game::bounceFromStrikers(QGraphicsItem *item, VectorXY *velocity)
{

    /*
     * This method bounces a graphic item from the striker and it affects its associated velocity vector passed down as a parameter by reference.
     * It takes into account the restitution of the striker.
     * It was reimplemented so items wouldnt get stuck on strikers (they get inside strikers and loop the cycle of negating the velocity)
     * This was achieved by only inverting the velocity when it hit the striker by moving towards it and from the inside of the field.
     * That is, items are free to enter the field if they are outside but if they are on the inside they will bounce off the strikers.
     * This was implemented by taking into account the sign of the velocity we want to negate upon bouncing.
    */



    if(!this->swappedStrikers) //strikers not swapped
    {
        //striker 1, bounce only if the item is moving down (positive y velocity)
        if(item->collidesWithItem(this->striker1) && velocity->getY() > 0)
        {
            velocity->setY(-1*velocity->getY()*this->striker1->getRestitution());

            if(item == this->puck)
            {
                this->puck->puckStrikerSound->play();
                this->hitStriker->play();
            }

        }

        //striker 2, bounce only if the item is moving up (negative y velocity)
        if(item->collidesWithItem(this->striker2) && velocity->getY() < 0)
        {
            velocity->setY(-1*velocity->getY()*this->striker2->getRestitution());

            if(item == this->puck)
            {
                this->puck->puckStrikerSound->play();
                this->hitStriker->play();
            }

        }
    }

    else if(this->swappedStrikers) //strikers swapped
    {
        if(item->collidesWithItem(this->striker2) && velocity->getY() > 0)
        {
            velocity->setY(-1*velocity->getY()*this->striker1->getRestitution());

            if(item == this->puck)
            {
                this->puck->puckStrikerSound->play();
                this->hitStriker->play();
            }

        }

        //striker 2, bounce only if the item is moving up (negative y velocity)
        if(item->collidesWithItem(this->striker1) && velocity->getY() < 0)
        {
            velocity->setY(-1*velocity->getY()*this->striker2->getRestitution());

            if(item == this->puck)
            {
                this->puck->puckStrikerSound->play();
                this->hitStriker->play();
            }

        }
    }



    //In both cases if the item is thepuck a sound is played.

    return;

}

void Game::moveItem(QGraphicsItem *item, VectorXY *velocity)
{
    /*
     * This method moves an item assuming constant velocity and passing the velocity vector associated with the item as a parameter.
     * It uses the cinematic formula in a timeStep-wise manner:
     * dr = ro + vdt
    */

    item->setPos(item->x()+this->timeStep*velocity->getX(),item->y()+this->timeStep*velocity->getY());

    return;
}

void Game::deleteBox(Box * box)
{
    /*
     * This method deletes a box from the game
     * First it removes it from the scene, and then it deallocates it from the memory.
    */
    this->scene->removeItem(box);
    delete box;
    return;
}

void Game::deleteAccelerator(Accelerator *accel)
{
    /*
     * This method deletes an accelerator from the game
     * First it removes it from the scene, and then it deallocates it from the memory.
    */
    this->scene->removeItem(accel);
    delete accel;
    return;
}

void Game::boxesCollidingWithPuck()
{
    /*
     * This methods deletes any box that has touched the puck.
     * And activates a random effect when it does so while playing a sound.
    */

    // we iterate over the items colliding with the puck
    for (int i = 0, n = this->puck->collidingItems().size();i < n;i++)
    {

        //if we find a box colliding with puck, we remove it, play a sound and activate an effect

        if(typeid (*(this->puck->collidingItems().at(i)))==typeid(Box))
        {
            //play sound
            this->hitBox->play();
            this->addBoxSound->play();

            //remove the box from the boxes vector
            this->boxes.removeOne((Box *)(this->puck->collidingItems().at(i)));

            //delete the box from the game
            this->deleteBox((Box *)(this->puck->collidingItems().at(i)));
            qDebug()<<"Box Deleted by Touch";

            //activate a random efect
            this->chooseRandomEffect();

            return;
        }
    }
}

void Game::attractorsCollidingWithPuck()
{
    /*
     * This methods deletes any accelerator that has touched the puck.
     * And it does so while playing a sound.
    */

    // we iterate over the items colliding with the puck
    for (int i = 0, n = this->puck->collidingItems().size();i < n;i++)
    {
        //if we find an accelerator colliding with puck, we remove it and play a sound
        if(typeid (*(this->puck->collidingItems().at(i)))==typeid(Accelerator))
        {
            //play sound
            this->hitAccel->play();
            this->addAccel->play();

            //remove accelerator from the accelerators vector
            this->accelerators.removeOne((Accelerator *) (this->puck->collidingItems().at(i)));

            //remove accelerator from the game
            this->deleteAccelerator((Accelerator *) (this->puck->collidingItems().at(i)));

            qDebug()<<"Accelerator Deleted by Touch";
            return;
        }
    }
}

void Game::chooseRandomEffect()
{
    /*
     * This method is the heart of the game.
     * It activates a random effect that affects the gameplay.
     * Currently there are 11 effects to choose from.
     * Some effects are permanent while other are timed.
     * The timed ones are implemented by changing the game and then restoring the changes after a period of time.
     * This is done using a single shot timer with restoration slots.
     * At each effect the narrators comments the event that happened.
     * Some effects incorporate sounds with them.
    */

    int effect = this->boundedRandomizer(1,11);
    qDebug()<<"Effect: "<<effect;
    QString comment;
    switch (effect)
    {
    case 1: //add a pack of accelerators near the puck (but not exactly in the same position since the puck gets expulsed instantly off the field because of the high acceleration)
    {
        this->addAccels->play();

        qreal safeZone = this->boundedRandomizer(50,100); //this safe zone prevents the accelerators from being added too much near the puck
        for(qint32 i = 0; i < ACCELERATOR_PACK; i++)
        {
            this->addAccelerator(this->puck->x()+safeZone*this->signRandomizer(),this->puck->y()+safeZone*this->signRandomizer());
        }

        comment = QString::number(ACCELERATOR_PACK) + QString(" Accelerators Added");
        this->narrator->narrate(comment);
        break;
    }
    case 2: //gives a random high velocity to the puck, it does so with a EFFECT_VELOCITY_BONUS so that its effect is much more clear to the player
    {
        this->velocify(this->puck->velocity,MIN_XV_PUCK+EFFECT_VELOCITY_BONUS,MAX_XV_PUCK+EFFECT_VELOCITY_BONUS,MIN_YV_PUCK+EFFECT_VELOCITY_BONUS,MAX_YV_PUCK+EFFECT_VELOCITY_BONUS);
        comment = QString("Puck Velocified");
        this->narrator->narrate(comment);
        break;
    }
    case 3: //change the randomly viscosity of the field for a period of time, if the viscosity chosen is too high there is an emergent gameplay where the puck moves and stops in an unpredictable manner
    {
        this->field->setViscosity(this->boundedRandomizer(1,100)*0.001);
        comment = QString("Field Viscosity Changed to: ") + QString::number(this->field->getViscosity());
        this->narrator->narrate(comment);
        QTimer::singleShot(VISCOSITY_TIME*1000,this,SLOT(restoreFieldViscosity()));
        break;
    }
    case 4: //makes the puck invisible for a period of time, Qt::transparent was not chosen because the puck could be on top of another item
    {
        this->setPuckColor(FIELD_COLOR);//Qt::transparent? What if puck is over somethinf else?
        comment = QString("Puck Invisible");
        this->narrator->narrate(comment);
        QTimer::singleShot(INVISIBLE_PUCK_TIME *1000,this,SLOT(setPuckVisible()));
        break;
    }
    case 5: //changes randomly the width of random player striker
    {
        this->multiplyStrikerWidthOfRandomPlayer(this->random10PercentMoreOrLess());
        //qDebug()<<"Random Striker Width Changed: "<<this->striker1->rect().width()<<","<<this->striker2->rect().width();
        comment = QString("Random Striker Width Changed");
        this->narrator->narrate(comment);
        break;
    }
    case 6: //changes randomly the witdh of a random player goal
    {
        this->multiplyGoalWidthOfRandomPlayer(this->random10PercentMoreOrLess());
        comment = QString("Random Goal Width Changed");
        this->narrator->narrate(comment);
        break;
    }
    case 7: //changes randomly the velocity of a random player striker
    {
        this->randomStrikerVelocityForRandomPlayer();
        comment = QString("Random Striker Velocity For Random Player");
        this->narrator->narrate(comment);
        break;
    }
    case 8: //negates the direction of a random player striker, it can only be reverted by luckily having the same effect again over the same player
    {
        this->negateRandomPlayerStrikerVelocity();
        comment = QString("Negate Striker Velocity for Random Player");
        this->narrator->narrate(comment);
        break;
    }
    case 9: //changes the restitution of all walls for a period of time
    {
        qreal r = this->randomRestitutionForAllWalls();
        comment = QString("Walls with restitution of ") + QString::number(r)+QString(" for ")+QString::number(WALL_RESTITUTION_TIME)+QString(" seconds");
        this->narrator->narrate(comment);
        QTimer::singleShot(WALL_RESTITUTION_TIME*1000,this,SLOT(restoreWallRestitution()));
        break;
    }
    case 10: //changes the restituion of all strikers for a period of time
    {
        qreal r = this->randomRestitutionForAllPlayers();
        comment = QString("Strikers with restitution of ") + QString::number(r)+QString(" for ") + QString::number(STRIKER_RESTITUTION_TIME) + QString(" seconds");
        this->narrator->narrate(comment);
        QTimer::singleShot(STRIKER_RESTITUTION_TIME*1000,this,SLOT(restoreStrikersRestitution()));
        break;
    }

    case 11: // swap the strikers of the players for a period of time
    {
        comment = QString("Striker switched for ") + QString::number(STRIKERS_SWAP_TIME) + QString(" seconds");
        this->narrator->narrate(comment);
        this->swapStrikers();
        QTimer::singleShot(STRIKERS_SWAP_TIME*1000,this,SLOT(swapBackStrikers()));
        break;
    }

    default: //no effect but this should not happen ever
        this->narrator->narrate("Why are you here?");
        break;
    }
}

void Game::addAccelerator(qreal x, qreal y)
{
    /*
     * This method is called by the boxes random effect
     * It adds a random accelerator to the game on a given position given by the x,y parameters
     * it can produces either repulsors or attractors
    */

    //add to accelerator vector with the origin the same as the scene origin (important for calculating the gravitational field)
    this->accelerators.append(new Accelerator(ACCELERATOR_RADIUS,this->signRandomizer()*ACCELERATOR_MASS,Qt::SolidPattern,Qt::white,0,0,0,0));

    //add to scene
    this->scene->addItem(this->accelerators.last());

    //set the wanted position
    this->accelerators.last()->setPos(x,y);

    //give it a random velocity
    this->velocify(this->accelerators.last()->velocity,MIN_XV_AandB,MAX_XV_AandB,MIN_YV_AandB,MAX_YV_AandB);

    //paint it according to its nature
    this->accelerators.last()->paintAccelerator();

    return;
}

void Game::setPuckColor(Qt::GlobalColor color)
{
    /*
     * This method just colors the puck.
     * It uses a method of the Puck Class to accomplish this.
    */
        this->puck->setColor(color);
        qDebug()<<"Puck colored";
        return;
}

void Game::multiplyStrikerWidthOfRandomPlayer(qreal gain)
{
    /*
     * This method just changes the striker width of a random player by multiplying it with a gain.
    */

    int player = this->generateRandomPlayer();

    qDebug()<<"player: "<<player<<" gain: "<<gain<<" 1: "<<this->striker1->rect().width()<<" 2: "<<this->striker2->rect().width();

    if(player == 1)
    {
        this->changeStrikerWidth(this->striker1,this->goal1,gain);
    }

    else if(player == 2)
    {
        this->changeStrikerWidth(this->striker2,this->goal2,gain);
    }

    return;
}

qreal Game::random10PercentMoreOrLess()
{
    /*
     * This method randomizes between 0.9 and 1.1 which is a gain of 10% less or more respectively
    */
    int dummy = this->boundedRandomizer(1,2);
    if(dummy == 1){qDebug()<<"gain: "<<0.9;return 0.9;}
    if(dummy == 2){qDebug()<<"gain: "<<1.1;return 1.1;}
    return 1;
}

void Game::changeStrikerWidth(Striker *striker, Goal * goal, qreal gain)
{
    /*
     * This method changes a striker width by multiplying it by a gain within the constraint of the associated goal.
     * The constraint is set by MIN_GOAL_TO_STRIKER_RATIO in the case of the gain being larger than 1.
     * This constraint prevents the striker being larger than the goal
     * in that case the player would be unbeatable by standing in front of the goal.
     *
     * When the gain is lower than 1 the constraint is that the width of the striker is at least the width of the scene divided by 32
    */

    if(gain > 1)
    {
        if (MIN_GOAL_TO_STRIKER_RATIO*striker->rect().width() < goal->getWidth()) //Minimum ratio is set by MIN_GOAL_TO_STRIKER_RATIO
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
        if (striker->rect().width() > this->width/32) //Minimum width is scene width/32
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
    /*
     * This method changes the goal width of a random player using a gain parameter.
    */

    int player = this->generateRandomPlayer();

    qDebug()<<"player: "<<player<<" gain: "<<gain<<" 1: "<<this->goal1->width<<" 2: "<<this->goal2->width;

    if(player == 1)
    {
        this->changeGoalWidth(this->goal1,this->striker1,gain);
    }

    else if(player == 2)
    {
        this->changeGoalWidth(this->goal2,this->striker2,gain);
    }

    return;

}


void Game::changeGoalWidth(Goal * goal,Striker * striker, qreal gain)
{

    /*
     * This method changes the width of a goal using a gain parameter and
     * within the constraints imposed by the associated striker.
    */


    if(gain > 1)
    {
        if (goal->getWidth() < this->width) //max goal width is the whole width of the scene
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
        if (goal->getWidth() > MIN_GOAL_TO_STRIKER_RATIO * striker->rect().width()) //min goal width is determined by the MIN_GOAL_TO_STRIKER_RATIO
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
    /* This method returns a random player (either 1 or 2) */
    return this->boundedRandomizer(1,2);
}

void Game::randomStrikerVelocityForRandomPlayer()
{
    /*
     * This method just assigns a random striker velocity to a random player.
     * It does so within parameter defined on compilation time.
    */

    int player = this->generateRandomPlayer();

    if(player==1)
    {
        this->striker1->velocity->setX(this->boundedRandomizer(MIN_STRIKER_VELOCITY,MAX_STRIKER_VELOCITY));
    }

    if(player==2)
    {
        this->striker2->velocity->setX(this->boundedRandomizer(MIN_STRIKER_VELOCITY,MAX_STRIKER_VELOCITY));
    }

    return;
}

void Game::negateRandomPlayerStrikerVelocity()
{
    /*
     * This method negates the striker velocity of a random player, so it moves in the opposite direction
    */

    int player = this->generateRandomPlayer();

    if(player==1)
    {
        this->striker1->negateVelocity();
    }
    else if(player==2)
    {
        this->striker2->negateVelocity();
    }
    return;
}

qreal Game::randomRestitutionForAllWalls()
{
    /*
     * This method gives a random restituion to all walls of the game.
     * It does so taking as a base a compilatio time parameter called BASE_RESTITUTION
     * It generates a random increment between 0.1 and 1.
     * So the range of the restituion is between BASE_RESTITUION + 0.1 to BASE_RESTITUTION + 1
     * It returns the number generated to get it commented by the narrator
    */

    qreal r = this->boundedRandomizer(1,10)*0.1;
    this->wallHD->setRestitution(BASE_RESTITUTION+r);
    this->wallHU->setRestitution(BASE_RESTITUTION+r);
    this->wallVL->setRestitution(BASE_RESTITUTION+r);
    this->wallVR->setRestitution(BASE_RESTITUTION+r);
    return BASE_RESTITUTION+r;
}

qreal Game::randomRestitutionForAllPlayers()
{
    /*
     * This method gives a random restitution to all of the players strikers.
     * It does so based on a BASE_RESTITUTION compilation time parameter.
     * And it generates a random increment "r" to add to this BASE_RESTITUION
     * r goes from 0.1 to 1.
     * So the range of the strikers restitution is
     * between BASE_RESTITUTION + 0.1 to BASE_RESTITUION + 1
     * It returns the number generated to get it commented by the narrator
    */

    qreal r = this->boundedRandomizer(1,10)*0.1;
    this->striker1->setRestitution(BASE_RESTITUTION+r);
    this->striker2->setRestitution(BASE_RESTITUTION+r);
    return BASE_RESTITUTION+r;
}

void Game::botsify(Striker * striker, bool dir)
{
    /*
     * This method moves the striker in a given direction within the limits of the field (given by the wall).
     * The direction is passed by a bot which has a dir attribute:
     * dir: false=left, true=right
    */

    if(puck->brush.color()!=FIELD_COLOR) //bots wont see when the puck is invisible as it happens for humans
    {
        // !dir is true when we want to move to the left
        // the collision here is with the vertical left wall (wallVL)
        if(!dir && !striker->collidesWithItem(this->wallVL))
        {
            striker->setX(striker->x()-this->timeStep*qFabs(striker->velocity->getX()));
        }

        // dir is true when we want to move to the right
        // the collision here is with the vertical right wall (wallVR)
        if(dir && !striker->collidesWithItem(this->wallVR))
        {
            striker->setX(striker->x()+this->timeStep*qFabs(striker->velocity->getX()));
        }
    }
        return;
}

bool Game::whereIsTheDamnPuckAskedTheBot(Striker *striker)
{
    /*
     * This method returns the direction where the puck is located
     * respect to the center of a given striker.
     *
     * If it is to the right it returns true.
     * If it is to the left it returns false.
     *
     * The center of a striker is it left x coordinate (pos().x()) plus half of its width.
     * So the value of that sum can be compared to the position of the puck to know where it is in respect to the striker.
    */

    if(puck->brush.color()!=FIELD_COLOR) //bots wont see when the puck is invisible as it happens for humans
    {
        if(striker->pos().x()+striker->rect().width()/2 > this->puck->pos().x())
        {
            return false;   //to the left
        }

        else if(striker->pos().x()+striker->rect().width()/2 < this->puck->pos().x())
        {
            return true;    //to the right
        }
    }

    return false; //this shouldnt be reached ever though

    /*
     * Also bots could move within the goal width.
     * This would result in harder bots.
     */
}

void Game::saveGame(QString filename)
{
    /*
     * This method saves the game in a file which name is passed as a parameter.
    */
    //pause game upon save
    this->pause = true;

    //generate file stream based on the name
    QFile file(filename);

    //open file in write text mode
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug()<< "error saving!";
        QMessageBox::critical(this,tr("ERROR"),tr("Error saving game!"));
        return;
    }

    qDebug() << "saving data:";

    if(this->swappedStrikers)
    {
        this->swapStrikers();
    }

    //write game data to file in a csv manner and each record separater by newline
    QTextStream out(&file);
    out << "GAME," << this->width << "," << this->height << "," << this->bot1->state << "," << this->bot1->level << "," << this->bot2->state << "," << this->bot2->level << "," << this->pause << ","<< this->maxScore<<","<<this->maxScoreStep<<"\n";
    out << "PUCK," << this->puck->x() << "," << this->puck->y() << "," << this->puck->velocity->getX() << "," << this->puck->velocity->getY() << "," << this->puck->acceleration->getX() << "," << this->puck->acceleration->getY() << "," << this->puck->radius << "\n";
    out << "STRIKER1," << this->striker1->x() << "," << this->striker1->y() << "," << this->striker1->velocity->getX() << "," << this->striker1->velocity->getY() << "," << this->striker1->rect().width() << "\n";
    out << "STRIKER2," << this->striker2->x() << "," << this->striker2->y() << "," << this->striker2->velocity->getX() << "," << this->striker2->velocity->getY() << "," << this->striker2->rect().width() << "\n";
    out << "GOAL1," << this->goal1->width << "\n";
    out << "GOAL2," << this->goal2->width << "\n";
    out << "SCORE1," << this->score1->getScore() << "\n";
    out << "SCORE2," << this->score2->getScore() << "\n";

    //accelerators
    for (qint32 i = 0; i < this->accelerators.size(); i++)
    {
        out << "ACCEL," << this->accelerators.at(i)->x() << "," << this->accelerators.at(i)->y() << "," << this->accelerators.at(i)->velocity->getX() << "," << this->accelerators.at(i)->velocity->getY() << "," << this->accelerators.at(i)->mass << "," << this->accelerators.at(i)->radius << "\n";
    }

    //boxes
    for(qint32 i = 0; i < this->boxes.size(); i++)
    {
        out << "BOX," << this->boxes.at(i)->x() << "," << this->boxes.at(i)->y() << "," << this->boxes.at(i)->velocity->getX() << "," << this->boxes.at(i)->velocity->getY() << "\n";
    }

    //end saving
    qDebug() << "data saved successfully";
    this->narrator->narrate("GAME SAVED!");
    file.close();

    return;
}

void Game::loadGame(QString filename)
{
    /*
     * This methods load the game from a file which name is passed as a parameter.
    */
    //pause game upon loading
    this->pause = true;

    if(this->swappedStrikers)
    {
        this->swapStrikers();
    }

    //open file in read text mode

    QFile file(filename);
        //qDebug() << "file opened:"<<file.open(QIODevice::ReadOnly);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug()<< "error loading!";
            QMessageBox::critical(this,tr("ERROR"),tr("Error loading game!"));
            return;
        }

        qDebug() << "loading data:";

        //load the data into a Qstring List using a buffer, this is because QString provides useful methods for loading.
        QString buffer;
        QList<QString> data;

        while(!file.atEnd())
        {

            buffer = file.readLine();
            //check if the line is empty
            if(buffer.length() > 3) //more than 3 because of the \r\n characters it appends upon saving
            {

                data.append(buffer);
                //remove the \r\ characters from the QString
                if(data.last().endsWith("\r\n"))
                {
                    data.last().remove("\r\n");
                }
                qDebug() << data.last();
            }

        }

        qDebug() << "data loaded successfully";

        qDebug() <<"cleaning game";

        //clean all the non permanent objects the game has

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

        //load data using the QList of QStrings loaded
        //we iterate over the QList looking for game entities, and with each different entity we load the corresponding values
        //the position of the values is determined by the save method

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
                this->striker2->setWidth(width);
           }

           else if(data.at(i).contains("GOAL1"))
           {
                qDebug() << "GOAL1:" << data.at(i).section(",",1);
                qreal width = data.at(i).section(",",1,1).toFloat();
                this->goal1->setWidth(width);

           }

           else if(data.at(i).contains("GOAL2"))
           {
                qDebug() << "GOAL2:" << data.at(i).section(",",1);
                qreal width = data.at(i).section(",",1,1).toFloat();
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

        //end of loading
        this->narrator->narrate("GAME LOADED!");
        qDebug() << "game loaded successfully";
}


void Game::readController1()
{
    /* This slot activates the read method of the associated controller, it gets triggered by the readyread signal of the corresponding serial port */
    this->control1->read(&(this->moveR1),&(this->moveL1),this->bot1->state);
    return;
}

void Game::readController2()
{
    /* This slot activates the read method of the associated controller, it gets triggered by the readyread signal of the corresponding serial port */
    this->control2->read(&(this->moveR2),&(this->moveL2),this->bot2->state);
    return;
}

void Game::newRound()
{
    /*
     * This slot starts a new round by centering the puck and giving it a velocity.
     * It gets activated by a single shot timer.
     * It narrates the event and unpauses the game (previously paused when the puck was outside the field)
    */

        this->narrator->narrate("NEW ROUND!");
        this->centerPuck();
        this->velocify(this->puck->velocity,MIN_XV_PUCK,MAX_XV_PUCK,MIN_YV_PUCK,MAX_YV_PUCK);
        this->pause = false;

}

void Game::swapStrikers()
{
    /*
     * This methods swaps the strikers of the players.
     * It accomplish this by interchanging the pointers to them and using a dummy pointer.
    */

    Striker * dummy;
    dummy = this->striker1;
    this->striker1 = this->striker2;
    this->striker2 = dummy;
    this->swappedStrikers = !this->swappedStrikers;
}

void Game::swapBackStrikers()
{
    /*
     * This method swap backs the strikers, triggered by a single shot timer
    */
    Striker * dummy;
    dummy = this->striker1;
    this->striker1 = this->striker2;
    this->striker2 = dummy;
    this->swappedStrikers = !this->swappedStrikers;
    this->narrator->narrate("Strikers swapped back!");
    this->effectEnds->play();
}


Game::~Game()
{
    //delete of all pointers allocated

    if(this->musicPlayer->state() == QMediaPlayer::PlayingState)
    {
        this->musicPlayer->stop();
    }


    if(this->swappedStrikers)
    {
        this->swapStrikers();
    }

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

    delete this->hitBox;
    delete this->addAccel;
    delete this->addAccels;
    delete this->addBoxSound;
    delete this->effectEnds;
    delete this->goalAt1Sound;
    delete this->goalAt2Sound;
    delete this->hitAccel;
    delete this->hitStriker;
    delete this->hitWall;
    delete this->winSound;



}

void Game::animate()
{
    /*
     * This slot animates the game each timeout of the motionTimer
    */

    //check if the game is pause
    if(!this->pause)
    {
        //check if bots are activated and give control to them if true
        if(this->bot1->state)
        {
            this->botsify(this->striker1,this->bot1->dir);
        }

        if(this->bot2->state)
        {
            this->botsify(this->striker2,this->bot2->dir);
        }

        //check items colliding with puck
        this->boxesCollidingWithPuck();
        this->attractorsCollidingWithPuck();

        //check goals
        this->scoreAtGoalCollision();
        this->markGoalAndRestart();

        //bounce and move everything
        this->bounceEverything();
        this->moveEverything();
        this->stopStrikersAtWallCollision();
        this->moveStrikers();
    }

}

void Game::addBox()
{
    /* This slot add a box to the game and its triggered by the boxTimer of the game */
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
    /* This slot restores the color of the puck, triggered by a single shot timer */
    this->puck->setColor(PUCK_COLOR);
    this->effectEnds->play();
    this->narrator->narrate(QString("Puck Visible"));
    return;
}

void Game::restoreFieldViscosity()
{
    /* This slot restores the viscosity of the fiel, triggered by a single shot timer */
    this->field->setViscosity(FIELD_VISCOSITY);
    this->effectEnds->play();
    this->narrator->narrate(QString("Field Viscosity Restored"));
    return;
}

void Game::restoreWallRestitution()
{
    /* This slot restores the restitution of all the game walls, triggered by a single shot timer */
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
    /* This slot restores the restution of all strikers, triggered by a single shot timer */
    this->striker1->setRestitution(1);
    this->striker2->setRestitution(1);
    this->effectEnds->play();
    this->narrator->narrate(QString("Strikers Restitution Restored"));
    return;
}

void Game::addAccelerator()
{
    /* This slot adds an accelerator to the game and its triggered by the acceleratorTimer of the game*/
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
    /* This slot is the reaction of the Bot and its triggered by the bot's reaction timer, only does so when the bot is activated */
    if(this->bot1->state)
    {
        this->bot1->dir = this->whereIsTheDamnPuckAskedTheBot(this->striker1);
    }
}

void Game::reactBot2()
{
    /* This slot is the reaction of the Bot and its triggered by the bot's reaction timer, only does so when the bot is activated */
    if(this->bot2->state)
    {
        this->bot2->dir = this->whereIsTheDamnPuckAskedTheBot(this->striker2);
    }
}
