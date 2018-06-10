#ifndef REGISTRO_H
#define REGISTRO_H

#include <QWidget>
#include"QtSql/qsqlquery.h"
#include"QtSql/QSqlError"
#include"QtSql/QSqlQuery"

//este hedader es el que registra los usuarios, y los guarda en una base de datos, que es creada aqui mismo.


namespace Ui {
class registro;
}

class registro : public QWidget
{
    Q_OBJECT

public:
    explicit registro(QWidget *parent = 0);
    ~registro();
    void CrearTabla();

private slots:
    void on_pushButton_clicked();

private:
    Ui::registro *ui;
};

#endif // REGISTRO_H
