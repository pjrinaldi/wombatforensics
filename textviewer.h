#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

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
    void UpdateEncoding(int unused);
    void GetTextContent(const QModelIndex &index);
signals:
    void HideTextViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::TextViewer* ui;
    TskObject tskobj;
    TskObject* tskptr;
    QByteArray txtdata;
    QByteArray tmpdata;
    QString decodedstring;
    qint64 curobjaddr;
    QList<QTextCodec*> codecs;
    void FindCodecs();
    QModelIndex curindex;
};

Q_DECLARE_METATYPE(TextViewer*);

#endif // TEXTVIEWER_H
