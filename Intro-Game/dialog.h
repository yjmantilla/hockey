#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include"registro.h"
#include"login.h"
#include"QtSql/qsqlquery.h"
#include"QtSql/QSqlError"
#include"QtSql/QSqlQuery"

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::dialog *ui;
    QSqlDatabase dbmain;
};

#endif // DIALOG_H
