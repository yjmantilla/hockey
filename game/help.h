#ifndef HELP_H
#define HELP_H

#include <QWidget>

namespace Ui {
class Help;
}

//Este header es el que despliega una ventana de instrucciones del juego

class Help : public QWidget
{
    Q_OBJECT

public:
    explicit Help(QWidget *parent = 0);
    ~Help();

private:
    Ui::Help *ui;
};

#endif // HELP_H
