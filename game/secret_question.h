#ifndef SECRET_QUESTION_H
#define SECRET_QUESTION_H

#include <QWidget>

#include"QtSql/qsqlquery.h"
#include"QtSql/QSqlError"
#include"QtSql/QSqlQuery"
#include <QWidget>
#include"mainwindow.h"

namespace Ui {
class Secret_question;
}

class Secret_question : public QWidget
{
    Q_OBJECT

public:
    explicit Secret_question(QWidget *parent = 0);
    ~Secret_question();

private slots:
    void on_commandLinkButton_clicked();

private:
    Ui::Secret_question *ui;
};

#endif // SECRET_QUESTION_H
