#include "ventana.h"
#include "ui_ventana.h"
#include "mainwindow.h"

Ventana::Ventana(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Ventana)
{
    ui->setupUi(this);
}

Ventana::~Ventana()
{
    delete ui;
}

void Ventana::on_actionReturn_To_Lobby_triggered()
{
    MainWindow *M =new MainWindow;       //Se Crea la variable MainWindow dentro de la función, siendo así una variable local
    M->show();                        //Se muestra el MainWindow
    close();
}
