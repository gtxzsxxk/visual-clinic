#include "../include/index.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /* 启动Index主页面 */
    Index w;
    w.show();
    return QApplication::exec();
}
