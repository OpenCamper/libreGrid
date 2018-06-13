#include "libregrid.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    libreGrid w;
    w.show();

    return app.exec();
}

