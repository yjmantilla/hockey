#include "game_window.h"
#include "ui_game_window.h"

Game_Window::Game_Window(QWidget *parent, QString filename, bool load, bool bot1, qreal bot1level, bool bot2,qreal bot2level, qint32 maxScore) :
    QMainWindow(parent),
    ui(new Ui::Game_Window)
  //Union deljuego con la ventana donde se repruduce, y genera las opciones de juego (menu, help, instrucciones, etc)
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

void Game_Window::on_actionExit_triggered() // Este boton es el que cierra el juego (destructor del juego),
                                            // y devuelve al menu principal.
{
    delete this->game;
    this->close();
}

void Game_Window::on_actionLoad_triggered()//Boton que carga una partida en el archivo que elija el usuario
{
    this->game->pause = true;
    this->game->loadGame(*(this->string));

}


void Game_Window::on_actionSave_triggered()//Boton que guarda una partida en el archivo que elija el usuario
{
    this->game->pause = true;
    this->game->saveGame(*(this->string));
}

void Game_Window::on_actionText_triggered()//Este boton abre una ventana donde el usuario debe ingresar el archivo
                                           //El cual se va a usar para guardar partias, cargarlas, o configurar controles.
{
    this->game->pause = true;
    get_string * getString = new get_string(this, this->string);

    getString->show();
}

void Game_Window::on_actionConfigure_Controller_1_triggered()//Boton que configura el control 1(usa el string ingresado en el insert text)
                                                             //Este lee el puerto donde se encuentra el arduino
{
    this->game->pause = true;
    this->game->control1->configure(*(this->string));
    connect(this->game->control1->serialPort,SIGNAL(readyRead()),this->game,SLOT(readController1()));

}

void Game_Window::on_actionConfigure_Controller_2_triggered()//Boton que configura el control 2(usa el string ingresado en el insert text)
                                                             //Este lee el puerto donde se encuentra el arduino
{
    this->game->pause = true;
    this->game->control2->configure(*(this->string));
    connect(this->game->control2->serialPort,SIGNAL(readyRead()),this->game,SLOT(readController2()));
}

void Game_Window::on_actionReset_triggered()//Este boton reinicia el juego, cambiando los datos a los inciales
{
    this->game->pause = true;
    this->game->loadGame("reset");
    this->game->narrator->narrate("FIGHT!");
}

void Game_Window::on_actionChange_Bot_1_Level_triggered()//Este boton configura el bot 1, comprobando su nivel de dificultad
                                                         //Tambien toma los datos ingresados en el insert text
{
    this->game->pause = true;
    this->game->bot1->changeBotLevel((*(this->string)).toDouble());
}

void Game_Window::on_actionChange_Bot_2_Level_triggered()//Este boton configura el bot 2, comprobando su nivel de dificultad
                                                         //Tambien toma los datos ingresados en el insert text
{
    this->game->pause = true;
    this->game->bot2->changeBotLevel((*(this->string)).toDouble());
}

void Game_Window::on_actionToggle_Bot_1_triggered()//Este boton pausa o inicia el bot 1
{
    this->game->pause = true;
    this->game->bot1->state = !this->game->bot1->state;
}

void Game_Window::on_actionToggle_Bot_2_triggered()//Este boton pausa o inicia el bot 2
{
    this->game->pause = true;
    this->game->bot2->state = !this->game->bot2->state;
}

void Game_Window::on_actionToggle_Pause_triggered()//Este boton pausa o inicia el juego
{
    this->game->pause = !this->game->pause;
}

void Game_Window::on_actionToggle_Music_triggered()//Este boton pausa o inicia la musica
{
    this->game->pause = true;
    if(this->game->musicPlayer->state() == QMediaPlayer::PlayingState)
    {
        this->game->musicPlayer->pause();
        qDebug() << "pause music";
    }

    else /*if (this->game->musicPlayer->PlayingState == QMediaPlayer::PausedState) This does not work for some reason...*/
    {
        this->game->musicPlayer->play();
        qDebug()<< "play music";
    }


}

void Game_Window::on_actioninstructions_triggered()// Este boton muestra o despliega una ventana de instrucciones de juego.
{
    Help * help = new Help();
    help->show();
}

void Game_Window::on_actionNext_Song_triggered()//Este boton cambia la cancion que esta sonando
{
    this->game->pause = true;
    this->game->playlist->next();
}

void Game_Window::on_actionShuffle_triggered()//Este boton cambia la cancion que esta sonando pero de forma aleatoria
{
    this->game->pause = true;
    this->game->playlist->shuffle();
}
