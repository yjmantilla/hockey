#include "get_string.h"
#include "ui_get_string.h"
#include <QDebug>


get_string::get_string(QWidget *parent, QString * filename) :
    QWidget(parent),
    ui(new Ui::get_string)
{
    ui->setupUi(this);
    this->ready = false;
    this->filename = filename;
    //this->filename_p = filename;
}

get_string::~get_string()
{
    delete ui;
}

void get_string::on_pushButton_clicked()
{

    qDebug() << "ok";
    *(this->filename) = this->ui->Nombre_arch->text();
    this->close();


}
