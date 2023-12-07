#include "tablestatedemo.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/qwidget.h>
#include <QtCore/qtemporaryfile.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TableStateDemo w;
    w.show();

    QTemporaryFile file("tmp.txt");

    return a.exec();
}
