#include "wombatinclude.h"
#include "wombatforensics.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void MyMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localmsg = msg.toLocal8Bit();
    switch(type)
    {
        case QtDebugMsg:
            fprintf(stderr, "My Debug: %s\n", localmsg.constData());
            break;
        case QtInfoMsg:
            fprintf(stderr, "My Info: %s\n", localmsg.constData());
            break;
        case QtWarningMsg:
            fprintf(stderr, "My Warning: %s\n", localmsg.constData());
            break;
        case QtCriticalMsg:
            fprintf(stderr, "My Critical: %s\n", localmsg.constData());
            break;
        case QtFatalMsg:
            fprintf(stderr, "My Fatal: %s\n", localmsg.constData());
            break;
    }
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(MyMessageOutput);
    QApplication a(argc, argv);
    
    //new Q_DebugStream(std::cout, ui->TXT_Console); // redirect console output to QTextEdit
    //Q_DebugStream::registerQDebugMessageHandler(); // redirect qDebug() output to QTextEdit
    a.setWindowIcon(QIcon(":/appicon"));
    a.setStyleSheet("QStatusBar::item { border: 0px solid black; }");
    WombatForensics w;
    w.show();
    
    return a.exec();
}
