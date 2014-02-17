#include "wombatinclude.h"
#include "wombatforensics.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WombatForensics w;
    w.show();
    
    return a.exec();
}
