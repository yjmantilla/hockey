#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include "game.h"
#include "get_string.h"
#include"help.h"

//Este header es el que reproduce la ventana donde se genera el juego.

namespace Ui {
class Game_Window;
}

class Game_Window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Game_Window(QWidget *parent = 0, QString filename = "", bool load = false, bool bot1=false, qreal bot1level = 1, bool bot2 = true, qreal bot2level=1, qint32 maxScore=21);
    ~Game_Window();
    Game * game;
    QString * string;

private slots:
    //Estos slots son los que interactua el usuario para guardar partida, cargar partidas, configurar controles,
    //entre otras funciones que tiene el juego.
    void on_actionExit_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionText_triggered();

    void on_actionConfigure_Controller_1_triggered();

    void on_actionConfigure_Controller_2_triggered();

    void on_actionReset_triggered();

    void on_actionChange_Bot_1_Level_triggered();

    void on_actionChange_Bot_2_Level_triggered();

    void on_actionToggle_Bot_1_triggered();

    void on_actionToggle_Bot_2_triggered();

    void on_actionToggle_Pause_triggered();

    void on_actionToggle_Music_triggered();

    void on_actioninstructions_triggered();

    void on_actionNext_Song_triggered();

    void on_actionShuffle_triggered();

private:
    Ui::Game_Window *ui;

};

#endif // GAME_WINDOW_H
