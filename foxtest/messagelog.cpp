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
    //hextext->setScrollStyle(VSCROLLER_NEVER|HSCROLLER_NEVER);
    /*
    msglog = new FXConsole(mainframe, this, ID_LOG, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    msglog->setBackColor(FX::colorFromName("white"));
    //msglog->drawTextFragment(this, 0, 0, 100, 1, "Test", 1, STYLE_TEXT);
    */
}
void MessageLog::AddMsg(FXString msg)
{
    msglog->appendText(msg);
}
