#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"
#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class ImageViewer : public FXDialogBox
{
    FXDECLARE(ImageViewer)

    private:
        FXVerticalFrame* vframe;
	FXImageView* imageview;

    protected:
        ImageViewer() {}

    public:
        ImageViewer(FXWindow* parent, const FXString& title);
	void LoadImage(ForImg* curforimg, FileItem* curfileitem);
};

#endif // IMAGEVIEWER_H
