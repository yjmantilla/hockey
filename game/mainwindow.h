#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "game.h"
#include "game_window.h"

#include <QMainWindow>


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
