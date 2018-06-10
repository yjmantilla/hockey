#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>

dialog::dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
    QString nombre;
    nombre="BaseDeDatos.sqlite";  //se genera el archivo tipo Sqlite
    dbmain=QSqlDatabase::addDatabase("QSQLITE");// se genera una base de datos en la base de Sql
    dbmain.setDatabaseName(nombre); // se agrega la base de datos al archivo

    if(dbmain.open()){
        qDebug()<<"Base de datos creada"; //comprobacion de que se genera la base de datos
    }
    else{
         qDebug()<<"Error";
    }
}

dialog::~dialog()
{
    delete ui;
}

void dialog::on_pushButton_clicked() //boton de la ventana que abre otra ventana para que el usuario se registre.
{
    registro * Registro = new registro();
    Registro->show();
    //close();
}

void dialog::on_pushButton_2_clicked() //boton de la ventana que abre otra ventana para que el usuario ingrese al juego.
{
    login *Login = new login();
    Login->show();
    //close();
}
