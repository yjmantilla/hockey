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
    QString consulta;//string donde se va a guardar los usuarios (en una tabla)

    //se agrega a la "consulta" la tabla del usuario.
    consulta.append("CREATE TABLE IF NOT EXISTS usuario("
                        "name VARCHAR(25),"
                        "password VARCHAR(25),"
                        "nick VARCHAR(25),"
                        "Question VARCHAR(25),"
                        "Answer VARCHAR(25) PRIMARY KEY"
                        ");");
    QSqlQuery crear;
    crear.prepare(consulta);//se crea la tabla de usuario con todas sus condiciones (name,nickname,password,etc.)
    if(crear.exec())
    {
        qDebug()<<"Tabla creada";
    }
    else
    {   qDebug()<<"Tabla no creada"<<crear.lastError();
    }
}

void registro::on_pushButton_clicked()//este boton agrega los usuaros al archivo se agregan los usuarios
{
    QString  user = ui->User->text();         //aqui se guarda el texto ingresado en el line edit en un string para poder agregarlos a las tabas de usuarios
    QString  password = ui->Password->text();
    QString  nick = ui->NickN->text();
    QString  Question = ui->Question->text();
    QString  Answer = ui->Answer->text();
    qDebug()<<user;
    qDebug()<<password;
    qDebug()<<nick;
    qDebug()<<Question;
    qDebug()<<Answer;
    QString insertar;
    insertar.append("INSERT INTO usuario(name,password,nick,Question,Answer)"
                    "values(:name,:password,:nick,:Question,:Answer)");

    QSqlQuery insertar_db; //aqui se inserta en la tabla cada una de las especificaciones de usuario
    insertar_db.prepare(insertar);
    insertar_db.bindValue(":name",user);
    insertar_db.bindValue(":password",password);
    insertar_db.bindValue(":nick",nick);
    insertar_db.bindValue(":Question",Question);
    insertar_db.bindValue(":Answer",Answer);
    if(insertar_db.exec())
    {
        qDebug()<<"Datos ingresados a la tabala";
        QMessageBox::information(this,tr("Success"),tr("Register done succesfully"));
    }
    else
    {
        qDebug()<<"Error al ingresar los datos"<<insertar_db.lastError();
        QMessageBox::critical(this,tr("ERROR"),tr("Nickname is already in use?"));
    }
}
