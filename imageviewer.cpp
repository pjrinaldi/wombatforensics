#include "imageviewer.h"

FXIMPLEMENT(ImageViewer,FXDialogBox, NULL, 0)

ImageViewer::ImageViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 300, 0,0,0,0, 0, 0)
{
    //plainfont = new FXFont(this->getApp(), "monospace");
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    imageview = new FXImageView(vframe);
    //imgview = new FXImageView(hsplitter);
    //textview = new FXText(vframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //textview->setFont(plainfont);
    //textview->setEditable(false);
}

void ImageViewer::LoadImage(ForImg* curforimg, FileItem* curfileitem)
{
    bool inmemory = true;
    uint8_t* tmpbuf = NULL;
    FILE* tmpfile;
    GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);
    if(!inmemory)
    {
	std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
	std::ifstream file(tmpfilestr.c_str(), std::ios::binary | std::ios::ate);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	tmpbuf = new uint8_t[size];
	file.read((char*)tmpbuf, size);
    }
    /*
    pdfdoc = poppler::document::load_from_raw_data((char*)tmpbuf, curfileitem->size);
    pagecnt = pdfdoc->pages();
    curfield->setText(FXString::value(pagenum + 1));
    totfield->setText(FXString::value(pagecnt));
    if(pagenum < pagecnt - 1)
	nextbutton->enable();
    // LOAD 1ST PAGE AS IMAGE
    LoadPage(0);
    */
    /*
    // code cleanup
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
    */
}
/*
void ImageViewer::GetText(FXString* buf)
{
    textview->getText(*buf);
}

void ImageViewer::GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags)
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
*/
