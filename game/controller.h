#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>


class Controller
{
public:
    Controller();
    Controller(QString name);
    QSerialPort * serialPort;
    char * data;

    void read(bool * moveR, bool * moveL, bool botState);
    ~Controller();
    void configure(QString name);
};

#endif // CONTROLLER_H
