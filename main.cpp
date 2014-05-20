#include "programme.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Programme w;
    w.show();

    return a.exec();
}
