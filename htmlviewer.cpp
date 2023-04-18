#include "htmlviewer.h"

FXIMPLEMENT(HtmlViewer,FXDialogBox, NULL, 0)

HtmlViewer::HtmlViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 300, 0,0,0,0, 0, 0)
{
    plainfont = new FXFont(this->getApp(), "monospace");
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    textview = new FXText(vframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    textview->setFont(plainfont);
    textview->setEditable(false);
}

void HtmlViewer::GetText(FXString* buf)
{
    textview->getText(*buf);
}

void HtmlViewer::GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags)
{
    // PLACE TITLE
    textview->appendText("Wombat Registry Report\n----------------------\n\n");
    // PLACE CONTENTS HEADER
    textview->appendText("Contents\n--------\n\n");
    // GENERATE TAGS AND THEIR COUNT
    for(int j=0; j < tags.size(); j++)
    {
        int tagcnt = 0;
        for(int i=0; i < taggedlist.no(); i++)
        {
            if(taggedlist.at(i).contains(tags.at(j).c_str()))
                tagcnt++;
        }
        textview->appendText(FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")\n");
    }
    textview->appendText("\n\n");
    // GENERATE TAG HEADER AND CONTENT
    for(int i=0; i < tags.size(); i++)
    {
        textview->appendText(FXString(tags.at(i).c_str()) + "\n");
        for(int j=0; j < tags.at(i).size(); j++)
            textview->appendText("-");
        textview->appendText("\n\n");
        for(int j=0; j < taggedlist.no(); j++)
        {
            std::size_t found = taggedlist.at(j).find("|");
            std::size_t rfound = taggedlist.at(j).rfind("|");
            FXString itemtag = taggedlist.at(j).mid(0, found);
            FXString itemhdr = taggedlist.at(j).mid(found+1, rfound - found - 1);
            FXString itemcon = taggedlist.at(j).mid(rfound+1, taggedlist.at(j).length() - rfound);
            if(itemtag == tags.at(i).c_str())
            {
                textview->appendText("Key:\t" + itemhdr + "\n");
                textview->appendText(itemcon + "\n");
                for(int k=0; k < itemcon.length(); k++)
                    textview->appendText("-");
                textview->appendText("\n\n");
            }
        }
    }
}
