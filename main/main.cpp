//#include "wombatforensics.h"
//#include <QtPlugin>
//#include <QApplication>
#include "wombatinclude.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WombatForensics w;
    w.show();
    
    return a.exec();
}
