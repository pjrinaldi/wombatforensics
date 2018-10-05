#include "wombatinclude.h"
#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
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
            //fprintf(stderr, "My Debug: %s\n", localmsg.constData());
            break;
        case QtInfoMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            //fprintf(stderr, "My Info: %s\n", localmsg.constData());
            break;
        case QtWarningMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            //fprintf(stderr, "My Warning: %s\n", localmsg.constData());
            break;
        case QtCriticalMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            //fprintf(stderr, "My Critical: %s\n", localmsg.constData());
            break;
        case QtFatalMsg:
            ba.append(tmpstring + "\t" + localmsg.constData() + "\n");
            //fprintf(stderr, "My Fatal: %s\n", localmsg.constData());
            break;
    }
    msglog->append(QString(tmpstring + ": " + localmsg.constData()));
    logfile.write(ba);
    //logfile.write(QString(tmpstring + "\t" + logmsg + "\n").toStdString().c_str());
    //logfile.close();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    a.setWindowIcon(QIcon(":/appicon"));
    //a.setStyleSheet("QStatusBar::item { border: 0px solid black; }");
    WombatForensics w;
    qInstallMessageHandler(MyMessageOutput);
    w.show();
    
    return a.exec();
}
