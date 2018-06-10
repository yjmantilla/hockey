#include "help.h"
#include "ui_help.h"

Help::Help(QWidget *parent) ://muestra la ventana donde estan las instrucciones de juego
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
}

Help::~Help()
{
    delete ui;
}
