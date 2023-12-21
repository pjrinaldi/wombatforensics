#include "thumbviewer.h"

FXIMPLEMENT(ThumbViewer,FXDialogBox, NULL, 0)

ThumbViewer::ThumbViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 800, 600, 0,0,0,0, 0, 0)
{
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    iconlist = new FXIconList(vframe, NULL, 0, ICONLIST_BIG_ICONS|LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
}

void ThumbViewer::LoadIcon(FXString iconpath)
{
    //std::cout << "iconpath: " << iconpath.text() << std::endl;
    FXIcon* tmpicon = new FXPNGIcon(this->getApp());
    FXFileStream stream;
    stream.open(iconpath, FXStreamLoad);
    tmpicon->loadPixels(stream);
    stream.close();
    tmpicon->create();
    iconlist->appendItem(iconpath.rafter('/').rbefore('.'), tmpicon, NULL);
}

void ThumbViewer::SetItemSpace(int itemspace)
{
    iconlist->setItemSpace(itemspace);
}
void ThumbViewer::Clear()
{
    iconlist->clearItems();
}
