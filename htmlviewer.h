#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_htmlviewer.h"

namespace Ui
{
    class HtmlViewer;
}

class HtmlViewer : public QDialog
{
    Q_OBJECT

public:
    HtmlViewer(QWidget* parent = 0);
    ~HtmlViewer();
    void ShowHtml(const QModelIndex &index);
    void LoadHtml(QString filepath);

private slots:
    void HideClicked();
    void GetHtmlContent(const QModelIndex &index);
    
signals:
    void HideHtmlViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::HtmlViewer* ui;
    TskObject tskobj;
    TskObject* tskptr;
    unsigned long long curobjaddr;
    QByteArray htmldata;
    
};

Q_DECLARE_METATYPE(HtmlViewer*);

#endif // HTMLVIEWER_H
