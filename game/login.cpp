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

void login::on_commandLinkButton_clicked()//Este boton es el que compara si el nickname y contraseña ingresado se encuentran en la base de datos.
{
   QString nick = ui->nick->text();//El texto que se ingresa en el line editor se guarda en una variable, para poder ser comparada en la base de datos.(nickname)
   QString password = ui->pass->text();//El texto que se ingresa en el line editor se guarda en una variable, para poder ser comparada en la base de datos.(contraseña)
   QSqlQuery buscar;//esta funcion es la que busca en la base de datos si existe e nickname
   QString password2;//Esta es la contraseña del usuario ingresado, la cual se va a comparar con la ingersada
   QString consulta;//en consulta es donde se pondra el nickname del usuario
   QString nombre;//aqui es donde se va a alojar el nickname que esta en la base de datos
   consulta.append("SELECT * FROM usuario where nick='"+nick+"'");
   buscar.prepare(consulta);//aqui compara los nickname de las base de datos
   if(buscar.exec())
   {
       qDebug()<<"Consulta realizada";
       while(buscar.next())//este while guarda la contraseña y el nombre del usuario(la que se encuentra en la base de datos)
       {
           password2=buscar.value(1).toByteArray().constData();
           nombre=buscar.value(0).toByteArray().constData();
       }

   }
   else
   {
       qDebug()<<"Error en consuta";
       QMessageBox::critical(this,tr("ERROR"),tr("That user does not exist"));
   }
   if(password2=="" || nombre=="")//este if se usa para prevenir que el usuario no ingrese ningun nombre o contraseña
   {
       qDebug()<<"Error en consuta";
       QMessageBox::critical(this,tr("ERROR"),tr("Enter nickname and password"));
   }
   else if(password2==password)//aqui ya se comparan las contraseñas, entre la ingresada y la que se encuentra en la base de datos
                               //y si es correcta lo envia al menu principal del juego
   {
       QMessageBox::information(this,tr("Welcome"),tr("Welcome: %1").arg(nombre));
       MainWindow * mainwindow = new MainWindow();
       mainwindow->show();
       close();

   }
   else//cuando la contraseña esta incorrecta muestra un mensaje de un error al ingresar
   {
       QMessageBox::critical(this,tr("ERROR"),tr("Incorrect Password").arg(nombre));
   }

}

void login::on_commandLinkButton_2_clicked()//este boton es el que da la opcion al usuario de ingresar la pregunta secreta

{
    Secret_question * secretQ = new Secret_question();
    secretQ->show();

}
