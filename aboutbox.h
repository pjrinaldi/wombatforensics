#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class AboutBox : public FXDialogBox
{
    FXDECLARE(AboutBox)

    private:
        FXVerticalFrame* mainframe;
	FXPNGImage* mainimage;
	FXImageFrame* imgframe;
        FXLabel* mainlabel;

    protected:
        AboutBox() {}

    public:
        AboutBox(FXWindow* parent, const FXString& title);
};

#endif // ABOUTBOX_H
