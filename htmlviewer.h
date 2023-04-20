#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

//#include <hubbub/hubbub.h>
#include <litehtml/litehtml.h>

#include "/usr/local/include/fox-1.7/fx.h"

class browserwindow;

class HtmlWidget : public FXCanvas
{
    FXDECLARE(HtmlWidget);

    /*
    protected:
	HtmlWidget();
    private:
	HtmlWidget(const HtmlWidget&);
	HtmlWidget &operator=(const HtmlWidget&);
    public:
	HtmlWidget(FXComposite* p, FXObject* tgt=nullptr, FXSelector sel=0, FXuint opts=FRAME_NORMAL, FXint x=0, FXint y=0, FXint w=0, FXint h=0)
	{
	    FXCanvas(p, tgt, sel, opts, x, y, w, h);
	};
    */
};

class HtmlViewer : public FXDialogBox
{
    FXDECLARE(HtmlViewer)

    private:
        FXVerticalFrame* vframe;
        FXText* textview;
        FXFont* plainfont;

    protected:
        HtmlViewer() {}
	HtmlWidget* hwidget;

    public:
        HtmlViewer(FXWindow* parent, const FXString& title);
        void GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags);
        void GetText(FXString* buf);
};

#endif // HTMLVIEWER_H
