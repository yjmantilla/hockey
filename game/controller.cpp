#include "controller.h"
#include <QDebug>
#include <QObject>

Controller::Controller()
{

    this->data = new char[1];
    this->serialPort = new QSerialPort();

}

Controller::Controller(QString name)
{
    /*Configure Port*/

    this->data = new char[1];
    this->serialPort = new QSerialPort();

    this->configure(name);

}

void Controller::read(bool *moveR, bool *moveL, bool botState)
{
    this->serialPort->getChar(this->data)   ;
    QString dummy(this->data);

    if(!botState)
    {
        if (dummy.contains("r"))
        {
        *moveR = true;
        }

        else if (dummy.contains("l"))
        {
        *moveL = true;
        }

        else if (dummy.contains("0"))
        {
        *moveR = false;
        *moveL = false;
        }
    }
    this->serialPort->clear();
}

Controller::~Controller()
{
    this->serialPort->close();
    delete this->serialPort;
    delete this->data;
}

void Controller::configure(QString name)
{

    this->serialPort->setPortName(name);

    if(this->serialPort->open(QIODevice::ReadOnly))
    {
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

    else
    {
        qDebug() << this->serialPort->errorString();
        QMessageBox::critical(NULL,QObject::tr("Error in controller"),QObject::tr("Error opening port"));
    }

}
