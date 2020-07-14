#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include "ui_htmlviewer.h"
#include "artifactparser.h"

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
    void ShowArtifact(int artifacttype, const QModelIndex &index);
    void LoadHtml(QString filepath);

private slots:
    void HideClicked();
    void Reload();
    
signals:
    void HideReportPreviewWindow(bool checkstate);

protected:
    void mousePressEvent(QMouseEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    Ui::HtmlViewer* ui;
    qint64 curobjaddr;
    QByteArray htmldata;
    
};

Q_DECLARE_METATYPE(HtmlViewer*);

#endif // HTMLVIEWER_H
