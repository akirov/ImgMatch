#include <QApplication>
#include "ImgMatchUI.h"
#include "Logger.h"


int main(int argc, char *argv[])
{
    int ret;

    LOG("Starting up.");

    QApplication a(argc, argv);
    ImgMatchUI w;
    w.show();
    ret = a.exec();

    LOG("Finishing normally.");

    return ret;
}
