#include "messagelog.h"

FXIMPLEMENT(MessageLog,FXDialogBox,NULL, 0)

MessageLog::MessageLog(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 600, 200, 0,0,0,0, 0, 0)
{
    plainfont = new FXFont(this->getApp(), "monospace");
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 2, 2, 2, 2);
    msglog = new FXText(mainframe, this, ID_LOG, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK);
    msglog->setFont(plainfont);
    msglog->setEditable(false);
    msglog->setScrollStyle(VSCROLLER_ALWAYS);
}
void MessageLog::AddMsg(FXString msg)
{
    FXString fullmsg = FXString(GetDateTime(dtbuf)) + " | " + msg + "\n";
    msglog->appendText(fullmsg);
}

MessageLog::~MessageLog()
{
    this->hide();
}
