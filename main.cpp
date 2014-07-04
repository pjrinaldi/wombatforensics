#include "wombatinclude.h"
#include "wombatforensics.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/appicon"));
    a.setStyleSheet("QStatusBar::item { border: 0px solid black; };");
    WombatForensics w;
    w.show();
    
    return a.exec();
}
