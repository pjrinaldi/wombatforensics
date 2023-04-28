#ifndef THUMBVIEWER_H
#define THUMBVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class ThumbViewer : public FXDialogBox
{
    FXDECLARE(ThumbViewer)

    private:
        FXVerticalFrame* vframe;
	FXIconList* iconlist;

    protected:
        ThumbViewer() {}

    public:
        ThumbViewer(FXWindow* parent, const FXString& title);
        void LoadIcon(FXString iconpath);
        void SetItemSpace(int itemspace);
	void Clear(void);
};

#endif // THUMBVIEWER_H
