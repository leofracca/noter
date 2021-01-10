#include "mainnoter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainNoter w;
    w.show();
    return a.exec();
}
