#ifndef PDFVIEWER_H
#define PDFVIEWER_H

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

#include "wombatinclude.h"
#include "globals.h"
#include "wombatfunctions.h"
#include <poppler-qt5.h>
#include "ui_pdfviewer.h"
#include "videoviewer.h"

namespace Ui
{
    //class ImageViewer;
    //class ImageWindow;
}

class PdfViewer : public QDialog
{
    Q_OBJECT

public:
    PdfViewer(QWidget* parent = 0);
    ~PdfViewer();
    void ShowPdf(QString objid);
private slots:
    void NextPage();
    void PrevPage();

private:
    Ui::PdfViewer* ui;
protected:
    Poppler::Document* pdfdoc;
    Poppler::Page* pdfpage;
    int pagenumber = 0;
    int pagecount = 0;
    void mousePressEvent(QMouseEvent* event);
};

Q_DECLARE_METATYPE(PdfViewer*)
#endif // PDFVIEWER_H
