#ifndef MESSAGEVIEWER_H

#define MESSAGEVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_messageviewer.h"
#include "wombatfunctions.h"

namespace Ui
{
    class MessageViewer;
}

class MessageViewer : public QDialog
{
    Q_OBJECT
        
public:
    MessageViewer(QWidget* parent = 0);
    ~MessageViewer();
    //QTextEdit* msglog;
    //void ShowMessage(const QModelIndex &index);
    
private slots:
    void HideClicked();
    void UpdateMessages(int chkstate);
    //void UpdateEncoding();
    //void GetTextContent();
signals:
    void HideMessageViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::MessageViewer* ui;
    //TskObject tskobj;
    //TskObject* tskptr;
    //QByteArray txtdata;
    //QString decodedstring;
    //int curobjid;
    //QList<QTextCodec*> codecs;
    //void FindCodecs();
};

Q_DECLARE_METATYPE(MessageViewer*);

#endif // MESSAGEVIEWER_H
