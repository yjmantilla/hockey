#ifndef CONTROLLER_H
#define CONTROLLER_H

/* Libraries */
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

/*
 * Controller Class
 *
 * It abstracts a controller with:
 * A serial port.
 * A buffer char to read data from it.
*/

class Controller
{
public:
/* Attributes */
    QSerialPort * serialPort;
    char * buffer;

/* Methods */

    /* Constructor */
    Controller();
    Controller(QString name);

    /* Functionality */
    void read(bool * moveR, bool * moveL, bool botState);    
    void configure(QString name);

    /* Destructor */
    ~Controller();
};

#endif // CONTROLLER_H
