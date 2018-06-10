#ifndef LOGIN_H
#define LOGIN_H

#include"QtSql/qsqlquery.h"
#include"QtSql/QSqlError"
#include"QtSql/QSqlQuery"
#include <QWidget>
#include"mainwindow.h"
#include"secret_question.h"

//Este header es el que muestra la ventana donde el usuario se loguea, con su nickname y su contraseña
//Ademas se tiene un boton donde el usuario puede ingresar una pregunta secreta con su debida respuesta,
//en caso de que haya olvidado su contraseña

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_commandLinkButton_clicked();

    void on_commandLinkButton_2_clicked();

private:
    Ui::login *ui;
};

#endif // LOGIN_H
