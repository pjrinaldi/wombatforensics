#include "propviewer.h"

FXIMPLEMENT(PropertyViewer,FXDialogBox, NULL, 0)

PropertyViewer::PropertyViewer(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 415, 515, 0,0,0,0, 0, 0)
{
    vframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0);
    //proptable = new FXTable(vframe, this, 0, TABLE_COL_SIZABLE|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_Y|LAYOUT_FILL_X);
    /*
    proptable = new FXTable(vframe, this, 0, TABLE_COL_SIZABLE|LAYOUT_TOP|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    proptable->setTableSize(10, 2);
    proptable->setColumnText(0, "Property");
    proptable->setColumnText(1, "Value");
    proptable->setColumnHeaderHeight(proptable->getColumnHeaderHeight() + 5);
    proptable->setRowHeaderWidth(0);
    //proptable->setHeight(this->getHeight() / 2);
    */
    textview = new FXText(vframe, NULL, 0, LAYOUT_LEFT|LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    textview->setHeight(50);
    textview->setEditable(false);
}

void PropertyViewer::LoadProp(FXString* configpath, FXString* pname, std::string* propstr)
{
    std::vector<char> ptbytes;
    FXString ptpath = *configpath;
    //std::cout << "config path: " << configpath->text() << std::endl;
    //std::cout << "pname: " << pname->text() << std::endl;
    if(pname->contains("FAT12") != 0)
    {
	ptpath += "fat12file.pt";
	std::ifstream ptfile(ptpath.text(), std::ios::binary | std::ios::ate);
	std::streamsize ptfilesize = ptfile.tellg();
	ptbytes.resize(ptfilesize);
	ptfile.seekg(0, std::ios::beg);
	ptfile.read(ptbytes.data(), ptfilesize);
	std::string ptfilestring(ptbytes.data(), ptfilesize);
	std::vector<std::string> proplist;
	proplist.clear();
	std::istringstream properties(ptfilestring);
	std::string propitem;
	while(getline(properties, propitem, ';'))
	    proplist.push_back(propitem);
	for(int i=0; i < proplist.size(); i++)
	    std::cout << "prop item " << i+1 << ": " << proplist.at(i) << std::endl;
	//std::cout << "pt file string: " << ptfilestring << std::endl;
    }
    else
	std::cout << "is not fat12" << std::endl;
    /* WHILE LOOP TO SPLIT ON ';', THEN LOOPING OVER THOSE TO SPLIT ON ',' EXAMPLE
    std::vector<std::string> dirlayoutlist;
    dirlayoutlist.clear();
    std::istringstream dirlayoutstream(curdirlayout);
    std::string curlayout;
    while(getline(dirlayoutstream, curlayout, ';'))
	dirlayoutlist.push_back(curlayout);
    for(int k=0; k < dirlayoutlist.size(); k++)
    {
	diroffset = 0;
	dirsize = 0;
	std::size_t layoutsplit = dirlayoutlist.at(k).find(",");
	diroffset = std::stoull(dirlayoutlist.at(k).substr(0, layoutsplit));
	dirsize = std::stoull(dirlayoutlist.at(k).substr(layoutsplit+1));
	if(k == 0 && curfileitem != NULL) // first dirlayout entry and not root directory
	{
	    diroffset = diroffset + 64; // skip . and .. directories
	    dirsize = dirsize - 64; // adjust read size for the 64 byte skip
	}

     */ 
}

/*
void PropertyViewer::LoadHex(ForImg* curforimg, FileItem* curfileitem)
{
    bool inmemory = false;
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
    // POPULATE CONTENT
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
    // POPULATE SLACK
    uint8_t* slkbuf = NULL;
    uint64_t slacksize = 0;
    GetFileSlack(curforimg, curfileitem, &slkbuf, &slacksize);
    if(slacksize > 0)
    {
	filecontents += "\nSLACK SPACE\n-----------\n";
	if(slacksize < 16)
	{
	    filecontents += "0000\t";
	    std::stringstream ss;
	    ss << std::hex << std::setfill('0');
	    for(int i=0; i < 16; i++)
	    {
		if(i < slacksize)
		    ss << std::setw(2) << ((uint)slkbuf[i]) << " ";
		else
		    ss << "   ";
	    }
	    filecontents += FXString(ss.str().c_str()).upper();
	    for(int i=0; i < slacksize; i++)
	    {
		if(isprint(slkbuf[i]))
		    filecontents += FXchar(reinterpret_cast<unsigned char>(slkbuf[i]));
		else
		    filecontents += ".";
	    }
	    filecontents += "\n";
	}
	else
	{
	    int linecount = slacksize / 16;
	    int linerem = slacksize % 16;
	    if(linerem > 0)
		linecount++;
	    for(int i=0; i < linecount; i++)
	    {
		std::stringstream ss;
		ss << std::hex << std::setfill('0') << std::setw(8) << i * 16 << "\t";
		for(int j=0; j < 16; j++)
		{
		    if(j + i * 16 < slacksize)
			ss << std::setw(2) << ((uint)slkbuf[j + i * 16]) << " ";
		    else
			ss << "   ";
		}
		filecontents += FXString(ss.str().c_str()).upper();
		for(int j=0; j < 16; j++)
		{
		    if(j + i * 16 < slacksize)
		    {
			if(isprint(slkbuf[j + i * 16]))
			    filecontents += FXchar(reinterpret_cast<unsigned char>(slkbuf[j + i * 16]));
			else
			    filecontents += ".";
		    }
		}
		filecontents += "\n";
	    }
	}
    }
    //AddFileSlack(curforimg, curfileitem, slkbuf, &filecontents);
    textview->setText(filecontents);
    if(!inmemory)
	fclose(tmpfile);
    delete[] tmpbuf;
    delete[] slkbuf;
}
*/
