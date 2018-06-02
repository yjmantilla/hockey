#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
     S = new Game_Window(0,"",false,false,1,true,1,21);
     S->show();
     close();
}

void MainWindow::on_pushButton_2_clicked()
{
   M = new Game_Window(0,"",false,false,1,false,1,21);
   M->show();
    close();
}

//solo bots
//cargar partida
