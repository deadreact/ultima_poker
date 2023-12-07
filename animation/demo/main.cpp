#include <QtWidgets/qapplication.h>
#include "form.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Form f;
    f.show();

    return app.exec();
}

