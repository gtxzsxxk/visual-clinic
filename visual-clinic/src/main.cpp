#include "../include/index.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Index w;
    w.show();
    return QApplication::exec();
}
