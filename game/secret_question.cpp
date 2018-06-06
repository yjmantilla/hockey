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

void Secret_question::on_commandLinkButton_clicked()
{
    QString Question= ui->Secret_Q->text();
    QString Answer = ui->Answer->text();
    QSqlQuery buscar;
    QString Answer2;
    QString consulta;
    QString nombre;
    consulta.append("SELECT * FROM usuario where Question='"+Question+"'");
    buscar.prepare(consulta);
    if(buscar.exec())
    {
        qDebug()<<"Consulta realizada";
        while(buscar.next())
        {
            Answer2=buscar.value(4).toByteArray().constData();
            nombre=buscar.value(0).toByteArray().constData();
        }
    }
    else
    {
        qDebug()<<"Error en consuta";
        QMessageBox::critical(this,tr("ERROR"),tr("Pregunta no existe"));
    }
    if(Answer2==""||nombre=="")
    {
        qDebug()<<"Error en consuta";
        QMessageBox::critical(this,tr("ERROR"),tr("Ingrese la pregunta y la respuesta"));
    }
    else if(Answer2==Answer)
    {
        QMessageBox::information(this,tr("Welcome"),tr("Welcome: %1").arg(nombre));
        MainWindow * mainwindow = new MainWindow();
        mainwindow->show();
        close();

    }
    else
    {
        QMessageBox::critical(this,tr("ERROR"),tr("Respuesta Incorrectra ").arg(nombre));
    }

}
