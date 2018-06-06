#include "game_window.h"
#include "ui_game_window.h"

Game_Window::Game_Window(QWidget *parent, QString filename, bool load, bool bot1, qreal bot1level, bool bot2,qreal bot2level, qint32 maxScore) :
    QMainWindow(parent),
    ui(new Ui::Game_Window)
{
    ui->setupUi(this);

    this->game = new Game(ui->widget,ui->widget->width(),ui->widget->height(),filename,load,bot1,bot1level,bot2,bot2level,maxScore);
    this->string = new QString("");

}

Game_Window::~Game_Window()
{
    delete game;

    delete ui;
}

void Game_Window::on_actionExit_triggered()
{
    delete this->game;
    this->close();
}

void Game_Window::on_actionLoad_triggered()
{
    this->game->pause = true;
    this->game->loadGame(*(this->string));

}


void Game_Window::on_actionSave_triggered()
{
    this->game->pause = true;
    this->game->saveGame(*(this->string));
}

void Game_Window::on_actionText_triggered()
{
    this->game->pause = true;
    get_string * getString = new get_string(this, this->string);

    getString->show();
}

void Game_Window::on_actionConfigure_Controller_1_triggered()
{
    this->game->pause = true;
    this->game->control1->configure(*(this->string));
    connect(this->game->control1->serialPort,SIGNAL(readyRead()),this->game,SLOT(readController1()));

}

void Game_Window::on_actionConfigure_Controller_2_triggered()
{
    this->game->pause = true;
    this->game->control2->configure(*(this->string));
    connect(this->game->control2->serialPort,SIGNAL(readyRead()),this->game,SLOT(readController2()));
}

void Game_Window::on_actionReset_triggered()
{
    this->game->pause = true;
    this->game->loadGame("reset");
    this->game->narrator->narrate("FIGHT!");
}

void Game_Window::on_actionChange_Bot_1_Level_triggered()
{
    this->game->pause = true;
    this->game->bot1->changeBotLevel((*(this->string)).toDouble());
}

void Game_Window::on_actionChange_Bot_2_Level_triggered()
{
    this->game->pause = true;
    this->game->bot2->changeBotLevel((*(this->string)).toDouble());
}

void Game_Window::on_actionToggle_Bot_1_triggered()
{
    this->game->pause = true;
    this->game->bot1->state = !this->game->bot1->state;
}

void Game_Window::on_actionToggle_Bot_2_triggered()
{
    this->game->pause = true;
    this->game->bot2->state = !this->game->bot2->state;
}

void Game_Window::on_actionToggle_Pause_triggered()
{
    this->game->pause = !this->game->pause;
}

void Game_Window::on_actionToggle_Music_triggered()
{
    if(this->game->musicPlayer->PlayingState == QMediaPlayer::PlayingState)
    {
        this->game->musicPlayer->pause();
    }
    else /*if(this->game->musicPlayer->PlayingState == QMediaPlayer::PausedState)*/
    {
        this->game->musicPlayer->play();
    }

}

void Game_Window::on_actioninstructions_triggered()
{
    Help * help = new Help();
    help->show();
}
