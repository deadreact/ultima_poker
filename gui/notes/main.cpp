#include "notespageview.h"
#include "notescompactview.h"
#include "notespageview.h"
#include <QApplication>
#include <QResource>
#include <QDebug>
#include <QProcess>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationVersion("0.0.0.0.1");

    bool resourceLoaded = QResource::registerResource("resources.dat");
    qDebug() << "resourceLoaded" << resourceLoaded;

    NotesPageView w1;
    w1.init();
    w1.show();

    NotesCompactView w;
    w.resetModelsFrom(&w1);
    w.init();
    w.show();
//    w.resetMarkersModel(w1.markersModel());


    auto r = a.exec();
    return r;
}
