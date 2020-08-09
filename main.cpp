#include "wombatinclude.h"
#include "wombatforensics.h"
#include <Magick++.h>

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void MyMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localmsg = msg.toLocal8Bit();
    QByteArray ba;
    ba.clear();
    QString tmpstring = QDateTime::currentDateTime().toString(QString("MM/dd/yyyy hh:mm:ss t"));
    switch(type)
    {
        case QtDebugMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            break;
        case QtInfoMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            break;
        case QtWarningMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            break;
        case QtCriticalMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            break;
        case QtFatalMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            break;
    }
    msglog->append(QString(tmpstring + ": " + localmsg.constData()));
    if(logfile.isOpen())
    {
        logfile.write(ba);
        logfile.flush();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/appicon"));
    Magick::InitializeMagick(*argv);
    //a.setStyleSheet("QStatusBar::item { border: 0px solid black; }");
    WombatForensics w;
    qInstallMessageHandler(MyMessageOutput); // comment out to view debug statements in terminal
    w.show();
    
    return a.exec();
}
