#include "thumbviewer.h"

FXIMPLEMENT(ThumbViewer,FXDialogBox, NULL, 0)

ThumbViewer::ThumbViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 800, 600, 0,0,0,0, 0, 0)
{
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    iconlist = new FXIconList(vframe, NULL, 0, ICONLIST_BIG_ICONS|LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //imageview = new FXImageView(vframe, NULL, NULL, 0, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
}

/*
void ThumbViewer::LoadImage(ForImg* curforimg, FileItem* curfileitem)
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
    try
    {
	Magick::Blob inblob(tmpbuf, curfileitem->size);
	Magick::Image inimage(inblob);
	inimage.magick("PNG");
	inimage.write("/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".png");
	FXImage* img = new FXPNGImage(this->getApp(), NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
	FXFileStream stream;
	this->getApp()->beginWaitCursor();
	stream.open(FXString(std::string("/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".png").c_str()), FXStreamLoad);
	img->loadPixels(stream);
	stream.close();
	img->create();
	imageview->setImage(img);
	this->getApp()->endWaitCursor();
    }
    catch(Magick::Exception &error)
    {
	std::cout << "error encoutered: " << error.what() << std::endl;
    }
    // code cleanup
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
}
*/
