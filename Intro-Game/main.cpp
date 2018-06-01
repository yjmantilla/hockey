#include "mainwindow.h"
#include <QApplication>
#include "dialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   // MainWindow w;
    dialog l;
    l.show();
   // w.show();

    return a.exec();
}
