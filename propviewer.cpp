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
    plainfont2 = new FXFont(parent->getApp(), "monospace");
    textview = new FXText(vframe, NULL, 0, LAYOUT_LEFT|LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    textview->setHeight(50);
    textview->setFont(plainfont2);
    textview->setEditable(false);
}

void PropertyViewer::LoadProp(FXString* configpath, FXString* pname, std::string* propstr, uint8_t ptype)
{
    FXString propstring = "";
    std::vector<char> ptbytes;
    FXString ptpath = *configpath;
    //std::cout << "config path: " << configpath->text() << std::endl;
    //std::cout << "pname: " << pname->text() << std::endl;
    if(ptype == 0x00)
    {
	// NEED TO DO MORE FOR FORENSIC IMG TYPES
	ptpath += "forimg";
    }
    if(ptype & 0x01)
    {
	if(pname->contains("FAT12") != 0)
	    ptpath += "fat12";
	else if(pname->contains("FAT16") != 0)
	    ptpath += "fat16";
    }
    if(ptype & 0x02)
	ptpath += "file";
    ptpath += ".pt";
    //std::cout << "ptfile: " << ptpath.text() << std::endl;
    std::ifstream ptfile(ptpath.text(), std::ios::binary | std::ios::ate);
    std::streamsize ptfilesize = ptfile.tellg();
    //std::cout << "ptfile size: " << ptfilesize << std::endl;
    if(ptfilesize > 0)
    {
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
	std::vector<std::string> propvalues;
	propvalues.clear();
	std::istringstream pvalues(*propstr);
	std::string pval;
	while(getline(pvalues, pval, '>'))
	    propvalues.push_back(pval);
	uint64_t maxpvaluelength = 0;
	for(int i=0; i < propvalues.size(); i++)
	{
	    if(maxpvaluelength < propvalues.at(i).size())
		maxpvaluelength = propvalues.at(i).size();
	}
	// GET PROPERTY LENGTH & DESCRIPTION LENGTH
	uint64_t proplength = 0;
	uint64_t desclength = 0;
	for(int i=0; i < proplist.size(); i++)
	{
	    std::size_t propsplit = proplist.at(i).find("|");
	    if(proplength < proplist.at(i).substr(0, propsplit).size())
		proplength = proplist.at(i).substr(0, propsplit).size();
	    if(desclength < proplist.at(i).substr(propsplit + 1).size())
		desclength = proplist.at(i).substr(propsplit + 1).size();
	}
	FXString pheader = "Property";
	FXString vheader = "|Value";
	FXString dheader = "|Description";
	FXString hrule = "";
	for(int i=pheader.length() - 1; i < proplength - 1; i++)
	    pheader += " ";
	for(int i=vheader.length() - 1; i < maxpvaluelength; i++)
	    vheader += " ";
	for(int i=0; i < proplength + maxpvaluelength + desclength + 2; i++)
	    hrule += "-";
	propstring = hrule + "\n" + pheader + vheader + dheader + "\n" + hrule + "\n";

	//std::cout << "max pvalue length: " << maxpvaluelength << std::endl;
	if(propvalues.size() > 0)
	{
	    for(int i=0; i < proplist.size(); i++)
	    {
		std::string ptitle = "";
		std::string pdescr = "";
		FXString pvalue = FXString(propvalues.at(i).c_str());
		//std::cout << "pvalue size: " << pvalue.length() << std::endl;
		//std::cout << "spaces to add: " << maxpvaluelength - pvalue.length() << std::endl;
		if(pvalue.length() < maxpvaluelength)
		{
		    //for(int i=pvalue.length() - 1; i < maxpvaluelength - 1; i++)
		    for(int i=pvalue.length(); i < maxpvaluelength; i++)
			pvalue += " ";
		}
		//std::cout << "new pvalue size: " << pvalue.length() << std::endl;
		std::size_t propsplit = proplist.at(i).find("|");
		ptitle = proplist.at(i).substr(0, propsplit);
		if(ptitle.length() < proplength)
		{
		    for(int i=ptitle.length() - 1; i < proplength - 1; i++)
			ptitle += " ";
		}
		pdescr = proplist.at(i).substr(propsplit + 1);
		if(pdescr.length() < desclength)
		{
		    for(int i=pdescr.length() - 1; i < desclength - 1; i++)
			pdescr += " ";
		}
		propstring += FXString(ptitle.c_str()) + "|" + pvalue + "|" + FXString(pdescr.c_str()) + "\n";
		//std::cout << ptitle << "|" << propvalues.at(i) << "|" << pdescr << std::endl; 
	    }
	    propstring += hrule + "\n";
	}
    }
    /*
    for(int i=0; i < proplist.size(); i++)
	std::cout << "prop item " << i+1 << ": " << proplist.at(i) << std::endl;
    for(int i=0; i < propvalues.size(); i++)
	std::cout << "propvalue " << i+1 << ": " << propvalues.at(i) << std::endl;
    */
    //std::cout << "pt file string: " << ptfilestring << std::endl;
    //}
    //else
	//std::cout << "is not fat12" << std::endl;
    textview->setText(propstring);
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
