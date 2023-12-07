#include "widget.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    int r;
    qDebug().setVerbosity(7);
    try {
        r = a.exec();
    } catch (std::out_of_range& ex) {
        qDebug() << ex.what() << __FILE__ << __LINE__;
        a.dumpObjectInfo();
    }
    return r;
}
