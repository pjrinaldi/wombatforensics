#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

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
        /*
        poppler::document* pdfdoc;
        poppler::page* pdfpage;
        poppler::image pdfimage;
        poppler::page_renderer pdfrender;
        */

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

	long LoadPrevious(FXObject*, FXSelector, void*);
        
	//long AddEvidence(FXObject*, FXSelector, void*);
        //void GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags);
        //void GetText(FXString* buf);
};

FXDEFMAP(PdfViewer) PdfViewerMap[]={
    FXMAPFUNC(SEL_COMMAND, PdfViewer::ID_PREV, PdfViewer::LoadPrevious),
    //FXMAPFUNC(SEL_COMMAND, PdfViewer::ID_ADDEVID, EvidenceManager::AddEvidence),
    //FXMAPFUNC(SEL_COMMAND, PdfViewer::ID_REMEVID, EvidenceManager::RemEvidence),
    //FXMAPFUNC(SEL_SELECTED, PdfViewer::ID_EVIDLIST, EvidenceManager::EvidenceSelected),
};

#endif // PDFVIEWER_H
