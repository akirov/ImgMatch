#include <QtGui/QApplication>
#include "ImgMatchUI.h"
#include "Logger.h"


int main(int argc, char *argv[])
{
    int ret;

    try
    {
        LOG("Starting up.");

        QApplication a(argc, argv);
        ImgMatchUI w;
        w.show();
        ret = a.exec();

        LOG("Finishing normally.");
    }
    catch(const std::string& err_str)
    {
        std::cerr << "Caught string exception: " << err_str << std::endl;
        ret = -1;
    }
    catch(...)
    {
        std::cerr << "Caught exception." << std::endl;
        ret = -1;
    }

    return ret;
}
