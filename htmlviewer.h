#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

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

private slots:
    void HideClicked();
    void GetHtmlContent();
    
signals:
    void HideHtmlViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::HtmlViewer* ui;
    TskObject tskobj;
    TskObject* tskptr;
    int curobjid;
    QByteArray htmldata;
    
};

Q_DECLARE_METATYPE(HtmlViewer*);

#endif // HTMLVIEWER_H
