#include <QCoreApplication>
#include "application.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    app();
    return a.exec();
}
