#include "secret_question.h"
#include "ui_secret_question.h"
#include "QMessageBox"
#include <QDebug>


Secret_question::Secret_question(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Secret_question)
{
    ui->setupUi(this);
}

Secret_question::~Secret_question()
{
    delete ui;
}

void Secret_question::on_commandLinkButton_clicked()//aqui se hace lo mismo que se hace cuando un usuario quiere ingresar normalmete,
                                                    //pero con la pregunta y respuesta secreta
{
    QString Question= ui->Secret_Q->text();//El texto que se ingresa en el line editor se guarda en una variable, para poder ser comparada en la base de datos.(pregunta secreta)
    QString Answer = ui->Answer->text();//El texto que se ingresa en el line editor se guarda en una variable, para poder ser comparada en la base de datos.(respuesta)
    QSqlQuery buscar;//esta funcion es la que busca en la base de datos si existe la pregunta
    QString Answer2;//Esta es la respuesta de la pregunta ingresada, la cual se va a comparar con la ingersada
    QString consulta;//en consulta es donde se pondra pregunta del usuario
    QString nombre;//aqui es donde se va a alojar el nickname que esta en la base de datos
    consulta.append("SELECT * FROM usuario where Question='"+Question+"'");
    buscar.prepare(consulta);//aqui compara los nickname de las base de datos
    if(buscar.exec())
    {
        qDebug()<<"Consulta realizada";
        while(buscar.next())//este while guarda la pregunta y respuesta del usuario(la que se encuentra en la base de datos)
        {
            Answer2=buscar.value(4).toByteArray().constData();
            nombre=buscar.value(0).toByteArray().constData();
        }
    }
    else
    {
        qDebug()<<"Error en consuta";
        QMessageBox::critical(this,tr("ERROR"),tr("That question does not exist"));
    }
    if(Answer2==""||nombre=="")//este if se usa para prevenir que el usuario no ingrese ninguna pregunta o respuesta
    {
        qDebug()<<"Error en consuta";
        QMessageBox::critical(this,tr("ERROR"),tr("Enter question and answer"));
    }
    else if(Answer2==Answer)//aqui ya se comparan las respuestas, entre la ingresada y la que se encuentra en la base de datos
                            //y si es correcta lo envia al menu principal del juego
    {
        QMessageBox::information(this,tr("Welcome"),tr("Welcome: %1").arg(nombre));
        MainWindow * mainwindow = new MainWindow();
        mainwindow->show();
        close();

    }
    else//cuando la contraseña esta incorrecta muestra un mensaje de un error al ingresar
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Wrong answer").arg(nombre));
    }

}
