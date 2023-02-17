#ifndef MESSAGELOG_H
#define MESSAGELOG_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "common.h"

#include "/usr/local/include/fox-1.7/fx.h"

class MessageLog : public FXDialogBox
{
    FXDECLARE(MessageLog)

    private:
        FXVerticalFrame* mainframe;
        FXText* msglog;
        FXFont* plainfont;
        char dtbuf[35];

    protected:
        MessageLog() {}
        ~MessageLog();

    public:
        enum
        {
            ID_LOG = 1,
            ID_LAST
        };
        MessageLog(FXWindow* parent, const FXString& title);
        void AddMsg(FXString msg);
};

#endif // MESSAGELOG_H
