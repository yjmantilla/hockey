#ifndef VENTANA_H
#define VENTANA_H

#include <QMainWindow>

namespace Ui {
class Ventana;
}

class Ventana : public QMainWindow
{
    Q_OBJECT

public:
    explicit Ventana(QWidget *parent = 0);
    ~Ventana();

private slots:
    void on_actionReturn_To_Lobby_triggered();

private:
    Ui::Ventana *ui;
};

#endif // VENTANA_H
