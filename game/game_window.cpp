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
    this->game->loadGame(*(this->string));

}


void Game_Window::on_actionSave_triggered()
{
    this->game->saveGame(*(this->string));
}

void Game_Window::on_actionText_triggered()
{
    this->game->pause = true;
    get_string * getString = new get_string(this, this->string);

    getString->show();
}
