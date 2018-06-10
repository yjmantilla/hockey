#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include"registro.h"
#include"login.h"
#include"QtSql/qsqlquery.h"
#include"QtSql/QSqlError"
#include"QtSql/QSqlQuery"
#include "mainwindow.h"

//Este header es el que ayuda a crear la ventana que le da la opcion al usuario ingresar o registrarse
//Se usaron diferentes librerias del SQL, que ayudo a crear la base de datos de usuarios

namespace Ui {
class dialog;
}

class dialog : public QWidget
{
    Q_OBJECT

public:
    explicit dialog(QWidget *parent = 0);
    ~dialog();

private slots:

    //Estos slots son los botones que se generan en la ventana y cada uno cumple una funcion que es registrarse o
    //ingresar a un usuario ya registrado.

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::dialog *ui;
    // se uso el "QSqlDatabase", para crear la base de datos para los usuarios
    QSqlDatabase dbmain;

};

#endif // DIALOG_H
