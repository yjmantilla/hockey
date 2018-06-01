#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>

dialog::dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
    QString nombre;
    nombre="BaseDeDatos.sqlite";
    dbmain=QSqlDatabase::addDatabase("QSQLITE");
    dbmain.setDatabaseName(nombre);
    if(dbmain.open()){
        qDebug()<<"Base de datos creada";
    }
    else{
         qDebug()<<"Error";
    }
}

dialog::~dialog()
{
    delete ui;
}

void dialog::on_pushButton_clicked()
{
    registro * Registro = new registro();
    Registro->show();
   // close();
}

void dialog::on_pushButton_2_clicked()
{
    login *Login = new login();
    Login->show();
    close();
}
