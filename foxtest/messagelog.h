#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

//#include "icons.h"

#include "/usr/local/include/fox-1.7/fx.h"

class MessageLog : public FXDialogBox
{
    FXDECLARE(MessageLog)

    private:
        FXVerticalFrame* mainframe;
        FXConsole* msglog;
	//FXPNGImage* mainimage;
	//FXImageFrame* imgframe;
        //FXLabel* mainlabel;
        //std::vector<std::string>* tags = NULL;

    protected:
        MessageLog() {}

        //long onSomeCommand(FXObject* sender, FXSelector sel, void* ptr);

    public:
        enum
        {
            ID_LOG = 1,
            ID_LAST
        };
        MessageLog(FXWindow* parent, const FXString& title);
};

#endif // MESSAGELOG_H
