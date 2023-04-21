#ifndef PROPVIEWER_H
#define PROPVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class PropertyViewer : public FXDialogBox
{
    FXDECLARE(PropertyViewer)

    private:
        FXVerticalFrame* vframe;
        FXText* textview;
        FXFont* plainfont;

    protected:
        PropertyViewer() {}

    public:
        PropertyViewer(FXWindow* parent, const FXString& title);
        //void LoadHex(ForImg* curforimg, FileItem* curfileitem);
};

#endif // PROPVIEWER_H
