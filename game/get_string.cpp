#include "get_string.h"
#include "ui_get_string.h"
#include <QDebug>


get_string::get_string(QWidget *parent, QString * gameString) :
    QWidget(parent),
    ui(new Ui::get_string)
{
    ui->setupUi(this);
    this->string = gameString;//Aqui se guarda el string ingresado en la linea de texto que ingreso el usuario
}

get_string::~get_string()
{
    delete ui;
}

void get_string::on_pushButton_clicked()
{

    qDebug() << "ok";//cuando el usuario oprime ok el texto que se ingreso en el insert text, se guarda para ser usado en funciones como
                     //cargar partidas, guradarlas y configurar controles o los bots.
    *(this->string) = this->ui->textString->text();
    this->close();

}
