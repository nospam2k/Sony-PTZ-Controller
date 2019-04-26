#include "mainwindow.h"
#include "sonyptzapp.h"

int main(int argc, char *argv[])
{
    SonyPTZApp a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
