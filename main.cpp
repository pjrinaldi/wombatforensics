#include "wombatinclude.h"
#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    //new Q_DebugStream(std::cout, ui->TXT_Console); // redirect console output to QTextEdit
    //Q_DebugStream::registerQDebugMessageHandler(); // redirect qDebug() output to QTextEdit
    a.setWindowIcon(QIcon(":/appicon"));
    a.setStyleSheet("QStatusBar::item { border: 0px solid black; }");
    WombatForensics w;
    w.show();
    
    return a.exec();
}
