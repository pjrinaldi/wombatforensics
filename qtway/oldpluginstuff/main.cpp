#include "wombatforensics.h"
#include <QtPlugin>
#include <QApplication>

Q_IMPORT_PLUGIN(BasicTools)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WombatForensics w;
    w.show();
    
    return a.exec();
}
