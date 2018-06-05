#include "login.h"
#include "ui_login.h"
#include "QMessageBox"
#include <QDebug>


login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_commandLinkButton_clicked()
{
   QString nick = ui->nick->text();
   QString password = ui->pass->text();
   QSqlQuery buscar;
   QString password2;
   QString consulta;
   QString nombre;
   consulta.append("SELECT * FROM usuario where nick='"+nick+"'");
   buscar.prepare(consulta);
   if(buscar.exec())
   {
       qDebug()<<"Consulta realizada";
       while(buscar.next())
       {
           password2=buscar.value(1).toByteArray().constData();
           nombre=buscar.value(0).toByteArray().constData();
       }

   }
   else
   {
       qDebug()<<"Error en consuta";
       QMessageBox::critical(this,tr("ERROR"),tr("Usuario no existe"));
   }
   if(password2=="" || nombre=="")
   {
       qDebug()<<"Error en consuta";
       QMessageBox::critical(this,tr("ERROR"),tr("Ingrese su Nickname y su Password"));
   }
   else if(password2==password)
   {
       QMessageBox::information(this,tr("Bienvenido"),tr("Bienvenido: %1").arg(nombre));
       MainWindow * mainwindow = new MainWindow();
       mainwindow->show();
       close();

   }
   else
   {
       QMessageBox::critical(this,tr("ERROR"),tr("Contraseña Incorrecta").arg(nombre));
   }

}

void login::on_commandLinkButton_2_clicked()
{
    Secret_question * secretQ = new Secret_question();
    secretQ->show();

}
