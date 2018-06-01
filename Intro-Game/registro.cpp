#include "registro.h"
#include "ui_registro.h"
#include<QDebug>
#include"QMessageBox"

registro::registro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registro)
{
    ui->setupUi(this);
    CrearTabla();
}

registro::~registro()
{
    delete ui;
}

void registro::CrearTabla(){
    QString consulta;
    consulta.append("CREATE TABLE IF NOT EXISTS usuario("
                        "name VARCHAR(25),"
                        "password VARCHAR(25),"
                        "nick VARCHAR(25) PRIMARY KEY"
                        ");");
    QSqlQuery crear;
    crear.prepare(consulta);
    if(crear.exec())
    {
        qDebug()<<"Tabla creada";
    }
    else
    {   qDebug()<<"Tabla no creada"<<crear.lastError();
    }
}

void registro::on_pushButton_clicked()
{
    QString  user = ui->User->text();
    QString  password = ui->Password->text();
    QString  nick = ui->NickN->text();
    qDebug()<<user;
    qDebug()<<password;
    qDebug()<<nick;
    QString insertar;
    insertar.append("INSERT INTO usuario(name,password,nick)"
                    "values(:name,:password,:nick)");

    QSqlQuery insertar_db;
    insertar_db.prepare(insertar);
    insertar_db.bindValue(":name",user);
    insertar_db.bindValue(":password",password);
    insertar_db.bindValue(":nick",nick);
    if(insertar_db.exec())
    {
        qDebug()<<"Datos ingresados a la tabala";
        QMessageBox::information(this,tr("Exito"),tr("Registro exitoso"));
    }
    else
    {
        qDebug()<<"Error al ingresar los datos"<<insertar_db.lastError();
        QMessageBox::critical(this,tr("ERROR"),tr("El Nickname ya esta en uso"));
    }
}
