#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <QMainWindow>
#include "game.h"
#include "get_string.h"

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
    void on_actionExit_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionText_triggered();

private:
    Ui::Game_Window *ui;
    //get_string * getFile;

};

#endif // GAME_WINDOW_H
