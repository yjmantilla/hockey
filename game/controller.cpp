#include "controller.h"
#include <QDebug>
#include <QObject>

Controller::Controller()
{

    /* Empty Constructor, just allocates memory needed */
    this->buffer = new char[1];
    this->serialPort = new QSerialPort();

}

Controller::Controller(QString name)
{
    /*Constructor that includes port configuration*/

    /* Memory Allocation */
    this->buffer = new char[1];
    this->serialPort = new QSerialPort();

    /* Configuration */
    this->configure(name);

}

void Controller::read(bool *moveR, bool *moveL, bool botState)
{
    /* Read 1 char from the serial to the buffer */
    this->serialPort->getChar(this->buffer);

    /* Make a QString from the buffer (QString provides useful methods) */
    QString bufferString(this->buffer);

    /* Move the Striker if the associated bot is not activated */
    if(!botState)
    {
        /* Move Right */
        if (bufferString.contains("r"))
        {
        *moveR = true;
        }
        /* Move Left */
        else if (bufferString.contains("l"))
        {
        *moveL = true;
        }
        /* Dont Move */
        else if (bufferString.contains("0"))
        {
        *moveR = false;
        *moveL = false;
        }
    }

    /*Clear information that arrived from the port, otherwise it gets stuck there (VERY IMPORTANT!)*/
    this->serialPort->clear();
}

Controller::~Controller()
{
    /* Destructor */

    /* Close port beforehand */
    this->serialPort->close();

    delete this->serialPort;
    delete this->buffer;
}

void Controller::configure(QString name)
{
    /* This methods configures a port based on its name */

    this->serialPort->setPortName(name);

    /* Try opening the port*/
    if(this->serialPort->open(QIODevice::ReadOnly))
    {
        /* If successful configure port attributes */
        /* On any error tell the user */

        if(!this->serialPort->setBaudRate(QSerialPort::Baud9600))
        {
            qDebug() << this->serialPort->errorString();
            QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error setting baud rate"));
        }

        if(!this->serialPort->setDataBits(QSerialPort::Data8))
        {
            qDebug() << this->serialPort->errorString();
            QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error setting data bits"));
        }

        if(!this->serialPort->setParity(QSerialPort::NoParity))
        {
            qDebug() << this->serialPort->errorString();
            QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error setting parity"));
        }

        if(!this->serialPort->setStopBits(QSerialPort::OneStop))
        {
            qDebug() << this->serialPort->errorString();
            QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error setting stop bits"));
        }

        if(!this->serialPort->setFlowControl(QSerialPort::NoFlowControl))
        {
            qDebug() << this->serialPort->errorString();
            QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error setting flow control"));
        }
    }

    /* Give another error if the port couldnt be opened with the given name*/
    else
    {
        qDebug() << this->serialPort->errorString();
        QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error opening port"));
    }

}
