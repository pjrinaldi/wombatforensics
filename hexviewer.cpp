#include "hexviewer.h"

FXIMPLEMENT(HexViewer,FXDialogBox, NULL, 0)

HexViewer::HexViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 700, 450, 0,0,0,0, 0, 0)
{
    plainfont = new FXFont(this->getApp(), "monospace");
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    textview = new FXText(vframe, NULL, 0, LAYOUT_FILL_X|LAYOUT_FILL_Y);
    textview->setFont(plainfont);
    textview->setEditable(false);
}

void HexViewer::LoadHex(ForImg* curforimg, FileItem* curfileitem)
{
    bool inmemory = true;
    uint8_t* tmpbuf = NULL;
    FILE* tmpfile;
    FXString filecontents = "";
    GetFileContent(curforimg, curfileitem, &inmemory, &tmpbuf, tmpfile);
    if(!inmemory)
    {
        std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
        std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        tmpbuf = new uint8_t[size];
        file.read((char*)tmpbuf, size);
    }
    if(curfileitem->size < 16)
    {
        filecontents += "0000\t";
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for(int i=0; i < 16; i++)
        {
            if(i < curfileitem->size)
                ss << std::setw(2) << ((uint)tmpbuf[i]) << " ";
            else
                ss << "   ";
        }
        filecontents += FXString(ss.str().c_str()).upper();
        for(int i=0; i < curfileitem->size; i++)
        {
            if(isprint(tmpbuf[i]))
                filecontents += FXchar(reinterpret_cast<unsigned char>(tmpbuf[i]));
            else
                filecontents += ".";
        }
        filecontents += "\n";
    }
    else
    {
        int linecount = curfileitem->size / 16;
        int linerem = curfileitem->size % 16;
        if(linerem > 0)
            linecount++;
        for(int i=0; i < linecount; i++)
        {
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(8) << i * 16 << "\t";
            for(int j=0; j < 16; j++)
            {
                if(j + i * 16 < curfileitem->size)
                    ss << std::setw(2) << ((uint)tmpbuf[j + i * 16]) << " ";
                else
                    ss << "   ";
            }
            filecontents += FXString(ss.str().c_str()).upper();
            for(int j=0; j < 16; j++)
            {
                if(j + i * 16 < curfileitem->size)
                {
                    if(isprint(tmpbuf[j + i * 16]))
                        filecontents += FXchar(reinterpret_cast<unsigned char>(tmpbuf[j + i * 16]));
                    else
                        filecontents += ".";
                }
            }
            filecontents += "\n";
        }
    }
    textview->setText(filecontents);
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
}

