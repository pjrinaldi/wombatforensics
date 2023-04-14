#include "pdfviewer.h"


FXIMPLEMENT(PdfViewer,FXDialogBox, PdfViewerMap, ARRAYNUMBER(PdfViewerMap))

PdfViewer::PdfViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 400, 300, 0,0,0,0, 0, 0)
{
    //plainfont = new FXFont(this->getApp(), "monospace");
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    hframe = new FXHorizontalFrame(vframe, LAYOUT_TOP|LAYOUT_FILL_X);
    previcon = new FXPNGIcon(this->getApp(), back16);
    prevbutton = new FXButton(hframe, "", previcon, this, ID_PREV, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 5, 5);
    nexticon = new FXPNGIcon(this->getApp(), frwd16);
    nextbutton = new FXButton(hframe, "", nexticon, this, ID_NEXT, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 5, 5);
    pglabel = new FXLabel(hframe, "Page:", NULL, JUSTIFY_CENTER_Y);
    curfield = new FXTextField(hframe, 4);
    oflabel = new FXLabel(hframe, "of");
    totfield = new FXTextField(hframe, 4);
    imgview = new FXImageView(vframe, NULL, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    prevbutton->disable();
    nextbutton->disable();
    //newicon = new FXPNGIcon(this->getApp(), documentnew);
    //newbutton = new FXButton(toolbar, "", newicon, this, ID_NEW, BUTTON_TOOLBAR|FRAME_RAISED, 0,0,0,0, 4,4,4,4);
    //addbutton = new FXButton(hframe1, "Add", NULL, this, ID_ADDEVID, FRAME_RAISED|FRAME_THICK, 0, 0, 0, 0, 20, 20);
    //textview = new FXText(vframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //textview->setFont(plainfont);
    //textview->setEditable(false);
}

long PdfViewer::LoadPrevPage(FXObject*, FXSelector, void*)
{
    std::cout << "load previous page if exists" << std::endl;

    return 1;
}

long PdfViewer::LoadNextPage(FXObject*, FXSelector, void*)
{
    std::cout << "load next page if exists" << std::endl;

    return 1;
}

void PdfViewer::LoadPdf(ForImg* curforimg, FileItem* curfileitem)
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
    pdfdoc = poppler::document::load_from_raw_data((char*)tmpbuf, curfileitem->size);
    pagecnt = pdfdoc->pages();
    pagenum = 1;
    curfield->setText(FXString::value(pagenum));
    totfield->setText(FXString::value(pagecnt));
    if(pagenum < pagecnt)
	nextbutton->enable();
    // LOAD 1ST PAGE AS IMAGE
    pdfpage = pdfdoc->create_page(0);
    pdfrender.set_render_hint(poppler::page_renderer::antialiasing, true);
    pdfrender.set_render_hint(poppler::page_renderer::text_antialiasing, true);
    pdfimage = pdfrender.render_page(pdfpage, 72.0, 72.0, -1, -1, -1, -1, poppler::rotate_0);
    pdfimage.save("/tmp/wf/tmp.png", "PNG");
    FXImage* img = new FXPNGImage(this->getApp(), NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
    FXFileStream stream;
    this->getApp()->beginWaitCursor();
    stream.open("/tmp/wf/tmp.png", FXStreamLoad);
    img->loadPixels(stream);
    stream.close();
    img->create();
    imgview->setImage(img);
    this->getApp()->endWaitCursor();
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
    //std::cout << "inmemory: " << inmemory << std::endl;
    /*
    bool inmemory = true;
    uint8_t* tmpbuf = NULL;
    FILE* tmpfile;
    std::string filecontents = "";
    GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);
    ParseArtifact(curforimg, &currentitem, curfileitem, &inmemory, tmpbuf, tmpfile, &filecontents);
    plaintext->setText(FXString(filecontents.c_str()));
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
    */
}

/*
void Viewer::GetText(FXString* buf)
{
    textview->getText(*buf);
}

void Viewer::GenerateReport(FXArray<FXString> taggedlist, std::vector<std::string> tags)
{

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
	poppler::document* pdfdoc;
	poppler::page* pdfpage;
	pdfdoc = poppler::document::load_from_raw_data((char*)tmpbuf, curfileitem->size);
	int pagecount = pdfdoc->pages();
	for(int i=0; i < pagecount; i++)
	    filecontents->append(pdfdoc->create_page(i)->text().to_latin1());
}*/
