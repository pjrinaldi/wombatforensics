#include "filters.h"

FXIMPLEMENT(Filters,FXDialogBox,NULL, 0)

Filters::Filters(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 600, 200, 0,0,0,0, 0, 0)
{
    //plainfont = new FXFont(this->getApp(), "monospace");
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 2, 2, 2, 2);
    sortlabel = new FXLabel(mainframe, "Show ID(s) that contain:", NULL, JUSTIFY_LEFT);
    idspinner = new FXSpinner(mainframe, 3);
    okbutton = new FXButton(mainframe, "Apply", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    //cancelbutton = new FXButton(hframe8, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    //savebutton = new FXButton(hframe8, "Save", NULL, this, FXDialogBox::ID_ACCEPT, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    idspinner->setValue(0);
    idspinner->setIncrement(1);
    //idspinner->setRange(0, 10);
    //idspinner->setRange(0, globalid);
    //pagespinner = new FXSpinner(topframe, 3, this, ID_PAGESPIN);
    //pagespinner->setRange(1, pagecount);
    //pagespinner->setValue(1);
    /*
    msglog = new FXText(mainframe, this, ID_LOG, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y|FRAME_SUNKEN|FRAME_THICK);
    msglog->setFont(plainfont);
    msglog->setEditable(false);
    msglog->setScrollStyle(VSCROLLER_ALWAYS);
    */
}

void Filters::SetRange(int maxrange)
{
    idspinner->setRange(1, maxrange);
}

void Filters::SetIndex(int colindex)
{
    columnindex = colindex;
}

std::string Filters::ReturnFilter(void)
{
    std::string filterstring = "";
    if(columnindex == 0) // CHECK COL
    {
    }
    else if(columnindex == 1) // ID COL
    {
        filterstring = std::to_string(idspinner->getValue());
    }
    return filterstring;
}

/*
void Filters::AddMsg(FXString msg)
{
    //FXString fullmsg = FXString(GetDateTime(dtbuf)) + " | " + msg + "\n";
    //msglog->appendText(fullmsg);
}
*/

Filters::~Filters()
{
    this->hide();
}
