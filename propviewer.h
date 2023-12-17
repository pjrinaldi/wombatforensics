#ifndef PROPVIEWER_H
#define PROPVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "fox-1.7/fx.h"

#include "libewf.h"

class PropertyViewer : public FXDialogBox
{
    FXDECLARE(PropertyViewer)

    private:
        FXVerticalFrame* vframe;
        //FXTable* proptable;
        FXFont* plainfont2;
        FXText* textview;

    protected:
        PropertyViewer() {}

    public:
        PropertyViewer(FXWindow* parent, const FXString& title);
	void LoadProp(FXString* configpath, FXString* pname, std::string* propstr, uint8_t ptype);
};

#endif // PROPVIEWER_H
