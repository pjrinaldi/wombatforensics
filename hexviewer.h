#ifndef HEXVIEWER_H
#define HEXVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class HexViewer : public FXDialogBox
{
    FXDECLARE(HexViewer)

    private:
        FXVerticalFrame* vframe;
        FXText* textview;
        FXFont* plainfont;

    protected:
        HexViewer() {}

    public:
        HexViewer(FXWindow* parent, const FXString& title);
        void LoadHex(ForImg* curforimg, FileItem* curfileitem);
};

#endif // HEXVIEWER_H
