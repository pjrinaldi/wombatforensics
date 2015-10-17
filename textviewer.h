#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2.1

#include "wombatinclude.h"
#include "globals.h"
#include "ui_textviewer.h"
#include "wombatfunctions.h"

namespace Ui
{
    class TextViewer;
}

class TextViewer : public QDialog
{
    Q_OBJECT
        
public:
    TextViewer(QWidget* parent = 0);
    ~TextViewer();
    void ShowText(const QModelIndex &index);
    
private slots:
    void HideClicked();
    void UpdateEncoding();
    void GetTextContent();
signals:
    void HideTextViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::TextViewer* ui;
    TskObject tskobj;
    TskObject* tskptr;
    QByteArray txtdata;
    QString decodedstring;
    unsigned long long curobjid;
    QList<QTextCodec*> codecs;
    void FindCodecs();
};

Q_DECLARE_METATYPE(TextViewer*);

#endif // TEXTVIEWER_H
