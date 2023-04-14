#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "/usr/include/poppler/cpp/poppler-document.h"
#include "/usr/include/poppler/cpp/poppler-page.h"
#include "/usr/include/poppler/cpp/poppler-page-renderer.h"
#include "/usr/include/poppler/cpp/poppler-image.h"

#include "/usr/local/include/fox-1.7/fx.h"

class PdfViewer : public FXDialogBox
{
    FXDECLARE(PdfViewer)

    private:
        FXVerticalFrame* vframe;
	FXHorizontalFrame* hframe;
        FXImageView* imgview;
        FXIcon* nexticon;
	FXButton* nextbutton;
        FXIcon* previcon;
	FXButton* prevbutton;
	FXLabel* pglabel;
	FXLabel* oflabel;
	FXTextField* curfield;
	FXTextField* totfield;
        poppler::document* pdfdoc;
	int pagenum = 0;
	int pagecnt = 0;

    protected:
        PdfViewer() {}

    public:
        enum
        {
	    ID_PREV = 100,
	    ID_NEXT = 101,
            ID_LAST
        };
        PdfViewer(FXWindow* parent, const FXString& title);
	void LoadPdf(ForImg* curforimg, FileItem* curfileitem);
	void LoadPage(int pnum);

	long LoadPrevPage(FXObject*, FXSelector, void*);
	long LoadNextPage(FXObject*, FXSelector, void*);
};

FXDEFMAP(PdfViewer) PdfViewerMap[]={
    FXMAPFUNC(SEL_COMMAND, PdfViewer::ID_PREV, PdfViewer::LoadPrevPage),
    FXMAPFUNC(SEL_COMMAND, PdfViewer::ID_NEXT, PdfViewer::LoadNextPage),
};

#endif // PDFVIEWER_H
