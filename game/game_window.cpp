#include "game_window.h"
#include "ui_game_window.h"

Game_Window::Game_Window(QWidget *parent, QString filename, bool load, bool bot1, qreal bot1level, bool bot2,qreal bot2level, qint32 maxScore) :
    QMainWindow(parent),
    ui(new Ui::Game_Window)
{
    ui->setupUi(this);

    this->juego = new Game(ui->widget,ui->widget->width(),ui->widget->height(),filename,load,bot1,bot1level,bot2,bot2level,maxScore);
    //this->juego = new Game(ui->widget,1280,720,"",false,false,1,true,1,21);

}

Game_Window::~Game_Window()
{
    delete juego;

    delete ui;
}

void Game_Window::on_actionExit_triggered()
{
    delete this->juego;
    this->close();
}

void Game_Window::on_actionLoad_triggered()
{
    get_string * getFile = new get_string(this);

    getFile->show();

//    if(getFile->ready)
//    {
//        this->juego->loadGame(getFile->filename);
//        getFile->close();
//    }



}
