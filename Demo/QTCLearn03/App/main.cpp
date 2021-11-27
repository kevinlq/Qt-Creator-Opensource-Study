#include <QApplication>
#include <QDebug>

#include "MiscUtil.h"
#include "Plugins.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << "hello";

    return a.exec();
}
