#include "chatsoft.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatSoft w;
    w.start();
    return a.exec();
}
