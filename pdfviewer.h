#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

//#include "hpdf.h"
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
        FXImageView* imgview;
        //FXText* textview;
        //FXFont* plainfont;
        /*
        HPDF_Doc pdf;
        HPDF_Page page;
        HPDF_Font defaultfont;
        HPDF_Font monofont;
        HPDF_REAL tw;
        HPDF_REAL height;
        HPDF_REAL width;
        HPDF_UINT i;
        poppler::document* pdfdoc;
        poppler::page* pdfpage;
        poppler::image pdfimage;
        poppler::page_renderer pdfrender;
        */

    protected:
        PdfViewer() {}

    public:
        PdfViewer(FXWindow* parent, const FXString& title);
        //void GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags);
        //void GetText(FXString* buf);
};

#endif // PDFVIEWER_H
