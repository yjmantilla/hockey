#include "get_string.h"
#include "ui_get_string.h"
#include <QDebug>


get_string::get_string(QWidget *parent, QString * gameString) :
    QWidget(parent),
    ui(new Ui::get_string)
{
    ui->setupUi(this);
    this->ready = false;
    this->string = gameString;
}

get_string::~get_string()
{
    delete ui;
}

void get_string::on_pushButton_clicked()
{

    qDebug() << "ok";
    *(this->string) = this->ui->textString->text();
    this->close();

}
