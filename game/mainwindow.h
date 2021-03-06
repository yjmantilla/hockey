#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "game.h"
#include "game_window.h"

#include <QMainWindow>

// este hedader genera la ventana donde el usuario elige si quiere jugar solo o con otra persona; y segun esto
// se enviara a la ventana donde se enera el juego con las especificaciones necesarias.
// change title, see https://stackoverflow.com/questions/10598269/how-to-change-the-title-of-the-window-in-qt
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    Game_Window *M;
    Game_Window *S;
};

#endif // MAINWINDOW_H
