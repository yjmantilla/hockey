#ifndef GET_STRING_H
#define GET_STRING_H

#include <QWidget>
#include "game_window.h"

//Este header genera la vetana donde se ingresa un texto que se usa para guardar archivos, cargarlos,
//y ademas configurar los controles (joystick).

namespace Ui {
class get_string;
}

class get_string : public QWidget
{
    Q_OBJECT

public:
    explicit get_string(QWidget *parent = 0, QString *gameString = NULL);
    ~get_string();
    QString * string;

private slots:
    void on_pushButton_clicked();

private:
    Ui::get_string *ui;
};

#endif // GET_STRING_H
