#include "pdfviewer.h"


FXIMPLEMENT(PdfViewer,FXDialogBox, NULL, 0)

PdfViewer::PdfViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 300, 0,0,0,0, 0, 0)
{
    //plainfont = new FXFont(this->getApp(), "monospace");
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    imgview = new FXImageView(vframe, NULL, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //textview = new FXText(vframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //textview->setFont(plainfont);
    //textview->setEditable(false);
}

/*
void Viewer::GetText(FXString* buf)
{
    textview->getText(*buf);
}

void Viewer::GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags)
{
    /*
    // GENERATE PDF
    pdf = HPDF_New(NULL, NULL);
    page = HPDF_AddPage(pdf);
    height = HPDF_Page_GetHeight(page);
    width = HPDF_Page_GetWidth(page);
    */
    // PLACE TITLE
/*    textview->appendText("Wombat Registry Report\n----------------------\n\n");
    /*
    monofont = HPDF_GetFont(pdf, "Courier", NULL);
    defaultfont = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_Page_SetFontAndSize(page, defaultfont, 24);
    //tw = HPDF_Page_TextWidth(page, "Wombat Registry Report");
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 10, height - 25);
    //HPDF_Page_MoveTextPos(page, (width - tw) / 2, height - 50);
    HPDF_Page_ShowText(page, "Wombat Registry Report");
    HPDF_Page_EndText(page);
    */
    // PLACE CONTENTS HEADER
/*    textview->appendText("Contents\n--------\n\n");
    /*
    HPDF_Page_BeginText(page);
    HPDF_Page_MoveTextPos(page, 10, height - 50);
    HPDF_Page_SetFontAndSize(page, defaultfont, 16);
    HPDF_Page_ShowText(page, "Contents");
    HPDF_Page_EndText(page);
    */
    // GENERATE TAGS AND THEIR COUNT
/*    for(int j=0; j < tags.size(); j++)
    {
        //FXString tagcontent = "";
        int tagcnt = 0;
        for(int i=0; i < taggedlist.no(); i++)
        {
            if(taggedlist.at(i).contains(tags.at(j).c_str()))
                tagcnt++;
        }
        textview->appendText(FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")\n");
        /*
        tagcontent += FXString(tags.at(j).c_str()) + " (" + FXString::value(tagcnt) + ")";
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 20, height - 60 - (j+1)*20);
        HPDF_Page_SetFontAndSize(page, defaultfont, 12);
        HPDF_Page_ShowText(page, tagcontent.text());
        HPDF_Page_EndText(page);
        */
/*    }
    textview->appendText("\n\n");
    // GENERATE TAG HEADER AND CONTENT
    //int curheight = 130;
    for(int i=0; i < tags.size(); i++)
    {
        textview->appendText(FXString(tags.at(i).c_str()) + "\n");
        for(int j=0; j < tags.at(i).size(); j++)
            textview->appendText("-");
        textview->appendText("\n\n");
        /*
        HPDF_Page_BeginText(page);
        HPDF_Page_MoveTextPos(page, 20, height - curheight);
        HPDF_Page_SetFontAndSize(page, defaultfont, 16);
        HPDF_Page_ShowText(page, tags.at(i).c_str());
        HPDF_Page_EndText(page);
        */
/*        for(int j=0; j < taggedlist.no(); j++)
        {
            //curheight += 30;
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
                /*
                HPDF_Page_BeginText(page);
                HPDF_Page_MoveTextPos(page, 20, height - curheight);
                HPDF_Page_SetFontAndSize(page, defaultfont, 12);
                HPDF_Page_ShowText(page, itemhdr.text());
                HPDF_Page_EndText(page);
                curheight += 20;
                std::string curstring = "";
                std::stringstream streamdata(itemcon.text());
                while(std::getline(streamdata, curstring, '\n'))
                {
                    HPDF_Page_BeginText(page);
                    HPDF_Page_MoveTextPos(page, 20, height - curheight);
                    HPDF_Page_SetFontAndSize(page, monofont, 12);
                    HPDF_Page_ShowText(page, curstring.c_str());
                    HPDF_Page_EndText(page);
                    curheight += 20;
                }
                */
/*            }
        }
        //curheight += 30;
    }

    /*
    HPDF_SaveToFile(pdf, "tmp.pdf");
    HPDF_Free(pdf);
    // RENDER PDF TO AN IMAGE
    pdfdoc = poppler::document::load_from_file("tmp.pdf");
    pdfpage = pdfdoc->create_page(0);
    pdfrender.set_render_hint(poppler::page_renderer::antialiasing, true);
    pdfrender.set_render_hint(poppler::page_renderer::text_antialiasing, true);
    pdfimage = pdfrender.render_page(pdfpage, 72.0, 72.0, -1, -1, -1, -1, poppler::rotate_0);
    pdfimage.save("tmp.png", "PNG");
    // Load Image
    FXImage* img = new FXPNGImage(this->getApp(), NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    FXFileStream stream;
    this->getApp()->beginWaitCursor();
    stream.open("tmp.png", FXStreamLoad);
    img->loadPixels(stream);
    stream.close();
    img->create();
    imgview->setImage(img);
    this->getApp()->endWaitCursor();
    */
/*
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }
	std::cout << "parse pdf here..." << std::endl;
	poppler::document* pdfdoc;
	poppler::page* pdfpage;
	pdfdoc = poppler::document::load_from_raw_data((char*)tmpbuf, curfileitem->size);
	int pagecount = pdfdoc->pages();
	for(int i=0; i < pagecount; i++)
	    filecontents->append(pdfdoc->create_page(i)->text().to_latin1());
 */ 


/*}*/
