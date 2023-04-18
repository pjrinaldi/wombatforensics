#ifndef HTMLVIEWER_H
#define HTMLVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include <hubbub/hubbub.h>

#include "/usr/local/include/fox-1.7/fx.h"

class HtmlViewer : public FXDialogBox
{
    FXDECLARE(HtmlViewer)

    private:
        FXVerticalFrame* vframe;
        FXText* textview;
        FXFont* plainfont;

    protected:
        HtmlViewer() {}

    public:
        HtmlViewer(FXWindow* parent, const FXString& title);
        void GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags);
        void GetText(FXString* buf);
};

#endif // HTMLVIEWER_H
