#include "artifactparser.h"

static lxb_html_token_t* token_callback(lxb_html_tokenizer_t *tkz, lxb_html_token_t *token, void *ctx)
{
    /* Skip all not #text tokens */
    if (token->tag_id != LXB_TAG__TEXT) {
        return token;
    }
    char htmlbuf[(int)(token->text_end - token->text_start)];
    sprintf(htmlbuf, "%.*s", (int) (token->text_end - token->text_start), token->text_start);
    ((std::string*)ctx)->append(htmlbuf);
    //printf("%.*s", (int) (token->text_end - token->text_start), token->text_start);

    return token;
}
void GetXmlText(rapidxml::xml_node<>* curnode, std::string* contents)
{
    if(curnode->type() == rapidxml::node_data || curnode->type() == rapidxml::node_cdata || curnode->type() == rapidxml::node_comment || curnode->type() == rapidxml::node_doctype)
    {
        contents->append(curnode->value());
        contents->append("\n");
    }
    rapidxml::xml_node<>* curchild = curnode->first_node();
    while(curchild != NULL)
    {
        GetXmlText(curchild, contents);
        curchild = curchild->next_sibling();
    }
}

void ParsePdf(FileItem* curfileitem, std::string* filecontents)
{
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    poppler::document* pdfdoc;
    poppler::page* pdfpage;
    pdfdoc = poppler::document::load_from_file(tmpfilestr);
    int pagecount = pdfdoc->pages();
    if(pagecount > 0)
        *filecontents = pdfdoc->create_page(0)->text().to_latin1();
}

void ParseHtml(uint8_t* prebuf, uint64_t bufsize, std::string* filecontents)
{   
    lxb_status_t status;
    lxb_html_tokenizer_t* tkz;
    tkz = lxb_html_tokenizer_create();
    status = lxb_html_tokenizer_init(tkz);
    lxb_html_tokenizer_callback_token_done_set(tkz, token_callback, filecontents);
    status = lxb_html_tokenizer_begin(tkz);
    status = lxb_html_tokenizer_chunk(tkz, prebuf, bufsize);
    status = lxb_html_tokenizer_end(tkz);
    lxb_html_tokenizer_destroy(tkz);
}

void ParseRecycler(FileItem* curfileitem, uint8_t* prebuf, uint64_t bufsize, std::string* filecontents)
{
    std::string titlestring = "INFO2 File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
    filecontents->clear();
    filecontents->append(titlestring + "\n");
    for(int i=0; i < titlestring.size(); i++)
        filecontents->append("-");
    filecontents->append("\n\n");
    uint32_t fileentrysize = 0;
    ReadInteger(prebuf, 12, &fileentrysize);
    int curpos = 20;
    while(curpos < bufsize)
    {
        uint8_t* fnamestr = new uint8_t[260];
        fnamestr = substr(prebuf, curpos, 260);
        uint32_t recycleindex = 0;
        ReadInteger(prebuf, curpos + 260, &recycleindex);
        uint32_t drivenumber = 0;
        ReadInteger(prebuf, curpos + 264, &drivenumber);
        filecontents->append("Index\t\t| " + std::to_string(recycleindex) + "\n");
        filecontents->append("File Path\t| " + std::string((char*)fnamestr) + "\n");
        delete[] fnamestr;
        std::string filenamestring = "";
        uint64_t deletedate = 0;
        ReadInteger(prebuf, curpos + 268, &deletedate);
        filecontents->append("Deleted Date\t| " + ConvertWindowsTimeToUnixTimeUTC(deletedate) + "\n");
        uint32_t fsize = 0;
        ReadInteger(prebuf, curpos + 276, &fsize);
        filecontents->append("File Size\t| " + ReturnFormattingSize(fsize) + " bytes\n\n");
        curpos = curpos + fileentrysize;
    }
}

void ParseRecycleBin(FileItem* curfileitem, uint8_t* prebuf, std::string* filecontents)
{
    std::string titlestring = "$I File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
    filecontents->clear();
    filecontents->append(titlestring + "\n");
    for(int i=0; i < titlestring.size(); i++)
        filecontents->append("-");
    filecontents->append("\n\n");
    uint64_t versionformat = 0;
    ReadInteger(prebuf, 0, &versionformat);
    uint64_t filesize = 0;
    ReadInteger(prebuf, 8, &filesize);
    uint64_t deletedate = 0;
    ReadInteger(prebuf, 16, &deletedate);
    uint32_t fnamesize = 0;
    uint32_t offset = 24;
    if(versionformat == 0x01)
        fnamesize = 520;
    else if(versionformat == 0x02)
    {
        uint32_t fnamesize = 0;
        ReadInteger(prebuf, 24, &fnamesize);
        offset = 28;
    }
    uint8_t* fnamestr = new uint8_t[fnamesize/2 +1];
    for(int i=0; i < fnamesize / 2; i++)
    {
        uint16_t singleletter = 0;
        ReadInteger(prebuf, offset + i*2, &singleletter);
        fnamestr[i] = (uint8_t)singleletter;
    }
    fnamestr[fnamesize] = 0;
    filecontents->append("File Path\t| " + std::string((char*)fnamestr) + "\n");
    filecontents->append("Deleted Date\t| " + ConvertWindowsTimeToUnixTimeUTC(deletedate) + "\n");
    filecontents->append("File Size\t| " + ReturnFormattingSize(filesize) + " bytes\n");
    delete[] fnamestr;
}

void ParsePreview(ForImg* curforimg, CurrentItem* curitem, FileItem* curfileitem, uint8_t* prebuf, uint64_t bufsize, std::string* filecontents, Magick::Image* previmg)
{
    if(curfileitem->sig.compare("Pdf") == 0) // PDF file
    { 
        ParsePdf(curfileitem, filecontents);
    }
    else if(curfileitem->sig.compare("Html") == 0) // HTML file
    {
        ParseHtml(prebuf, bufsize, filecontents);
    }
    else if(curfileitem->sig.compare("Recycler") == 0) // INFO2 file
    {
        ParseRecycler(curfileitem, prebuf, bufsize, filecontents);
    }
    else if(curfileitem->sig.compare("Recycle.Bin") == 0) // $I file
    {
        ParseRecycleBin(curfileitem, prebuf, filecontents);
    }
    else // partial hex preview
    {
    }
    /*
    if(curfileitem->cat.compare("Image") == 0)
    {
	if(plaintext->shown() || imgview->shown())
	{
	    plaintext->hide();
	    imgview->show();
	    try
	    {
		Magick::Blob inblob(tmpbuf, curfileitem->size);
		Magick::Image inimage(inblob);
		inimage.magick("PNG");
		inimage.write("/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".png");
		FXImage* img = new FXPNGImage(this->getApp(), NULL, IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
		FXFileStream stream;
		stream.open(FXString(std::string("/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".png").c_str()), FXStreamLoad);
		img->loadPixels(stream);
		stream.close();
		img->create();
                this->getApp()->beginWaitCursor();
		imgview->setImage(img);
		imgview->update();
                this->getApp()->endWaitCursor();
	    }
	    catch(Magick::Exception &error)
	    {
		std::cout << "error encoutered: " << error.what() << std::endl;
	    }
	}
    }
    */
}

void ParseArtifact(ForImg* curforimg, CurrentItem* curitem, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents)
{
    //std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
    std::string tmpfilestr = "/tmp/wf/" + std::to_string(curfileitem->gid) + "-" + curfileitem->name + ".tmp";
    tmpfilestr.erase(std::remove(tmpfilestr.begin(), tmpfilestr.end(), '$'), tmpfilestr.end());
    if(curfileitem->sig.compare("Microsoft Word 2007+") == 0) // word document
    {
        zip_error_t err;
        int interr = 0;
        int zipfileid = 0;
        std::string zipfilename = "";
        zip_uint64_t zipfilesize = 0;
        zip_t* curzip = NULL;
        if(inmemory)
        {
            zip_source_t* zipsrc = zip_source_buffer_create(tmpbuf, curfileitem->size, 1, &err);
            curzip = zip_open_from_source(zipsrc, ZIP_RDONLY, &err);
        }
        else
        {
            curzip = zip_open(tmpfilestr.c_str(), ZIP_RDONLY, &interr);
        }
        int64_t zipentrycnt = zip_get_num_entries(curzip, 0);
        //std::cout << "zip entries count: " << zipentrycnt << std::endl;
        struct zip_stat zipstat;
        zip_stat_init(&zipstat);
        for(int i=0; i < zipentrycnt; i++)
        {
            zip_stat_index(curzip, i, 0, &zipstat);
            if(strcmp(zipstat.name, "word/document.xml") == 0)
            {
                zipfileid = i;
                zipfilename = zipstat.name;
                zipfilesize = zipstat.size;
            }
        }
        //std::cout << "zipfileid: " << zipfileid << std::endl;
        zip_file_t* docxml = zip_fopen_index(curzip, zipfileid, ZIP_FL_UNCHANGED);
        char zipfilebuf[zipfilesize+1];
        zip_int64_t bytesread = zip_fread(docxml, zipfilebuf, zipfilesize);
        zipfilebuf[zipfilesize] = 0;
        interr = zip_fclose(docxml);

        rapidxml::xml_document<> worddoc;
        worddoc.parse<0>(zipfilebuf);
        rapidxml::xml_node<>* rootnode = worddoc.first_node();
        GetXmlText(rootnode, filecontents);
        //std::cout << filecontents << std::endl;
    }
    else if(curfileitem->sig.compare("Shortcut") == 0) // lnk file
    {
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }
        uint32_t flags = 0;
        uint32_t attributes = 0;
        uint64_t created = 0;
        uint64_t modified = 0;
        uint64_t accessed = 0;
        uint32_t filesize = 0;
        uint16_t shellstructurelength = 0;
        uint32_t voloffset = 0;
        uint32_t volstructurelength = 0;
        uint32_t voltype = 0;
        uint32_t volserial = 0;
        uint32_t volnameoffset = 0;
        uint32_t basepathoffset = 0;
        uint32_t networkvoloffset = 0;
        uint32_t remainingpathoffset = 0;
        uint32_t totalstructurelength = 0;
        std::string volnamestr = "";
        std::string basepathstr = "";
        std::string descstring = "";
        std::string relpathstr = "";
        std::string workingdirectory  = "";
        std::string commandstring = "";
        std::string iconstring = "";
        int curoffset = 0;

        std::string titlestring = "LNK File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
        filecontents->clear();
        filecontents->append(titlestring + "\n");
        for(int i=0; i < titlestring.size(); i++)
            filecontents->append("-");
        filecontents->append("\n\n");

        ReadInteger(tmpbuf, 20, &flags);
        //if(flags & 0x80)
        //    std::cout << "data strings are unicode rather than ascii." << std::endl;
        //std::cout << "flags: " << std::hex << flags << std::dec << std::endl;
        ReadInteger(tmpbuf, 24, &attributes);
        filecontents->append("File Attributes\t\t| ");
        if(attributes & 0x01)
            filecontents->append("Read Only,");
        if(attributes & 0x02)
            filecontents->append("Hidden,");
        if(attributes & 0x04)
            filecontents->append("System,");
        if(attributes & 0x10)
            filecontents->append("Directory,");
        if(attributes & 0x20)
            filecontents->append("Archived,");
        if(attributes & 0x80)
            filecontents->append("Normal");
        if(attributes & 0x100)
            filecontents->append("Temporary,");
        if(attributes & 0x200)
            filecontents->append("Sparse,");
        if(attributes & 0x400)
            filecontents->append("Reparse Point||Symbolic Link,");
        if(attributes & 0x800)
            filecontents->append("Compressed,");
        if(attributes & 0x1000)
            filecontents->append("Offline,");
        if(attributes & 2000)
            filecontents->append("Not Indexed,");
        if(attributes & 4000)
            filecontents->append("Encrypted,");
        if(attributes & 10000)
            filecontents->append("Virtual");
        filecontents->append("\n");
        //std::cout << "attributes: " << std::hex << attributes << std::dec << std::endl;
        ReadInteger(tmpbuf, 28, &created);
        //std::cout << "Created: " << ConvertWindowsTimeToUnixTimeUTC(created) << std::endl;
        ReadInteger(tmpbuf, 36, &modified);
        ReadInteger(tmpbuf, 44, &accessed);
        ReadInteger(tmpbuf, 52, &filesize);
        filecontents->append("Created\t\t\t| " + ConvertWindowsTimeToUnixTimeUTC(created) + "\n");
        filecontents->append("Modified\t\t| " + ConvertWindowsTimeToUnixTimeUTC(modified) + "\n");
        filecontents->append("Accessed\t\t| " + ConvertWindowsTimeToUnixTimeUTC(accessed) + "\n");
        filecontents->append("File Size\t\t| " + ReturnFormattingSize(filesize) + " bytes\n\n");
        if(flags & 0x01) // SHELL ITEM ID LIST IS PRESENT
        {
            // PARSE SHELL ID LIST HERE
            ReadInteger(tmpbuf, 76, &shellstructurelength);
	    libfwsi_item_list_t* itemlist = NULL;
	    libfwsi_error_t* itemerror = NULL;
	    int ret = libfwsi_item_list_initialize(&itemlist, &itemerror);
	    uint8_t* itemstream = new uint8_t[shellstructurelength];
	    itemstream = substr(tmpbuf, 78, shellstructurelength);
	    ret = libfwsi_item_list_copy_from_byte_stream(itemlist, itemstream, shellstructurelength, LIBFWSI_CODEPAGE_ASCII, &itemerror);
	    int itemlistcount = 0;
	    ret = libfwsi_item_list_get_number_of_items(itemlist, &itemlistcount, &itemerror);
	    //std::cout << "number of list items: " << itemlistcount << std::endl;
	    for(int i=0; i < itemlistcount; i++) // parse each shell item
	    {
                filecontents->append("Shell Item\t\t| " + std::to_string(i+1) + "\n");
                filecontents->append("\tItem Type\t| ");
		libfwsi_item_t* curitem;
		ret = libfwsi_item_initialize(&curitem, &itemerror);
		ret = libfwsi_item_list_get_item(itemlist, i, &curitem, &itemerror);
		int itemtype = 0;
		ret = libfwsi_item_get_type(curitem, &itemtype, &itemerror);
		//std::cout << "item type: " << itemtype << std::endl;
		uint8_t classtype = 0;
		ret = libfwsi_item_get_class_type(curitem, &classtype, &itemerror);
		//std::cout << "class type: " << (uint)classtype << std::endl;
                if(itemtype == 0)
                    filecontents->append("Unknown");
                else if(itemtype == 1)
                    filecontents->append("CDBurn");
                else if(itemtype == 2)
                    filecontents->append("Compressed Folder");
                else if(itemtype == 3)
                    filecontents->append("Control Panel");
                else if(itemtype == 4)
                    filecontents->append("Control Panel Category");
                else if(itemtype == 5)
                    filecontents->append("Control Panel CPL File");
                else if(itemtype == 6)
                    filecontents->append("Delegate");
                else if(itemtype == 7)
                    filecontents->append("File Entry");
                else if(itemtype == 8)
                    filecontents->append("Game Folder");
                else if(itemtype == 9)
                    filecontents->append("MTP File Entry");
                else if(itemtype == 10)
                    filecontents->append("MTP Volume");
                else if(itemtype == 11)
                    filecontents->append("Network Location");
                else if(itemtype == 12)
                    filecontents->append("Root Folder");
                else if(itemtype == 13)
                    filecontents->append("URI");
                else if(itemtype == 14)
                    filecontents->append("URI Sub Values");
                else if(itemtype == 15)
                    filecontents->append("User's Property View");
                else if(itemtype == 16)
                    filecontents->append("Volume");
                filecontents->append("\n");
                filecontents->append("\tClass Type\t| ");
                if((uint)classtype < 32)
                {
                    filecontents->append("Root Folder\n");
                    uint8_t* rootguid = new uint8_t[16];
                    libfwsi_root_folder_get_shell_folder_identifier(curitem, rootguid, 16, &itemerror);
		    filecontents->append("\tIdentifier\t| " + ReturnFormattedGuid(rootguid) + "\n");
                    filecontents->append("\tFolder Nmae\t| " + std::string(libfwsi_shell_folder_identifier_get_name(rootguid)) + "\n");
                    delete[] rootguid;
                }
                else if((uint)classtype < 48 && (uint)classtype >= 32)
                {
                    filecontents->append("Volume\n");
                    size_t volnamesize = libfwsi_volume_get_utf8_name_size(curitem, &volnamesize, &itemerror);
                    //std::cout << "vol name size: " << volnamesize << std::endl;
                    uint8_t* volname = new uint8_t[volnamesize+1];
                    ret = libfwsi_volume_get_utf8_name(curitem, volname, volnamesize, &itemerror);
                    volname[volnamesize] = 0;
                    filecontents->append("\tVolume Name\t| " + std::string((char*)volname) + ":\\ \n");
		    delete[] volname;
                }
                else if((uint)classtype < 64 && (uint)classtype >= 48)
                {
                    filecontents->append("File Entry\n");
                    size_t filenamesize = 0;
                    ret = libfwsi_file_entry_get_utf8_name_size(curitem, &filenamesize, &itemerror);
                    uint8_t* fname = new uint8_t[filenamesize+1];
                    ret = libfwsi_file_entry_get_utf8_name(curitem, fname, filenamesize, &itemerror);
                    fname[filenamesize] = 0;
                    filecontents->append("\tName\t\t| " + std::string((char*)fname) + "\n");
		    delete[] fname;
                    uint32_t fatdatetime = 0;
                    ret = libfwsi_file_entry_get_modification_time(curitem, &fatdatetime, &itemerror);
		    uint8_t* fdt = (uint8_t*)&fatdatetime;
		    uint16_t fatdate = (uint16_t)fdt[0] | (uint16_t)fdt[1] << 8;
		    uint16_t fattime = (uint16_t)fdt[2] | (uint16_t)fdt[3] << 8;
		    filecontents->append("\tModified Time\t| " + ConvertDosTimeToHuman(&fatdate, &fattime) + " UTC\n");
                    uint32_t attrflags = 0;
                    ret = libfwsi_file_entry_get_file_attribute_flags(curitem, &attrflags, &itemerror);
		    filecontents->append("\tFile Attributes\t| ");
		    if(attrflags & 0x01)
			filecontents->append("Read Only,");
		    if(attrflags & 0x02)
			filecontents->append("Hidden,");
		    if(attrflags & 0x04)
			filecontents->append("System,");
		    if(attrflags & 0x10)
			filecontents->append("Directory,");
		    if(attrflags & 0x20)
			filecontents->append("Archive,");
		    if(attrflags & 0x40)
			filecontents->append("Device,");
		    if(attrflags & 0x80)
			filecontents->append("Normal,");
		    if(attrflags & 0x100)
			filecontents->append("Temporary,");
		    if(attrflags & 0x200)
			filecontents->append("Sparse,");
		    if(attrflags & 0x400)
			filecontents->append("Reparse Point,");
		    if(attrflags & 0x800)
			filecontents->append("Commpressed,");
		    if(attrflags & 0x1000)
			filecontents->append("Offline,");
		    if(attrflags & 0x2000)
			filecontents->append("Not Indexed,");
		    if(attrflags & 0x4000)
			filecontents->append("Encrypted,");
		    if(attrflags & 0x10000)
			filecontents->append("Virtual");
		    filecontents->append("\n");
                }
                else if((uint)classtype < 80 && (uint)classtype >= 64)
                {
                    filecontents->append("Network Location\n");
		    size_t locsize = 0;
		    ret = libfwsi_network_location_get_utf8_location_size(curitem, &locsize, &itemerror);
		    uint8_t* locname = new uint8_t[locsize+1];
		    ret = libfwsi_network_location_get_utf8_location(curitem, locname, locsize, &itemerror);
		    locname[locsize] = 0;
		    filecontents->append("\tLocation\t| " + std::string((char*)locname) + "\n");
		    delete[] locname;
		    size_t descsize = 0;
		    ret = libfwsi_network_location_get_utf8_description_size(curitem, &descsize, &itemerror);
		    uint8_t* descname = new uint8_t[descsize + 1];
		    ret = libfwsi_network_location_get_utf8_description(curitem, descname, descsize, &itemerror);
		    descname[descsize] = 0;
		    filecontents->append("\tDescription\t| " + std::string((char*)descname) + "\n");
		    delete[] descname;
		    size_t comsize = 0;
		    ret = libfwsi_network_location_get_utf8_comments_size(curitem, &comsize, &itemerror);
		    uint8_t* comname = new uint8_t[comsize+1];
		    ret = libfwsi_network_location_get_utf8_comments(curitem, comname, comsize, &itemerror);
		    comname[comsize] = 0;
		    filecontents->append("\tComments\t| " + std::string((char*)comname) + "\n");
		    delete[] comname;
                }
                else if((uint)classtype < 96 && (uint)classtype >= 80)
                {
                    filecontents->append("URI\n");
                }
		
                int extblkcount = 0;
                ret = libfwsi_item_get_number_of_extension_blocks(curitem, &extblkcount, &itemerror);
                //std::cout << "extblkcnt: " << extblkcount << std::endl;
                for(int j=0; j < extblkcount; j++)
                {
                    libfwsi_extension_block_t* curblock = NULL;
                    libfwsi_item_get_extension_block(curitem, j, &curblock, &itemerror);
		    uint32_t extsig = 0;
		    ret = libfwsi_extension_block_get_signature(curblock, &extsig, &itemerror);
		    std::stringstream extstream;
		    extstream << std::hex << extsig;
		    if(extsig == 0xbeef0004)
		    {
			filecontents->append("Extension Block\t\t| " + std::to_string(j+1) + "\n");
			filecontents->append("\tSignature\t| 0x" + extstream.str() + " File Entry Extension\n");
		    }
		    uint32_t creationtime = 0;
		    ret = libfwsi_file_entry_extension_get_creation_time(curblock, &creationtime, &itemerror);
		    uint8_t* cdt = (uint8_t*)&creationtime;
		    uint16_t cdate = (uint16_t)cdt[0] | (uint16_t)cdt[1] << 8;
		    uint16_t ctime = (uint16_t)cdt[2] | (uint16_t)cdt[3] << 8;
		    filecontents->append("\tCreated Time\t| " + ConvertDosTimeToHuman(&cdate, &ctime) + " UTC\n");
		    uint32_t accesstime = 0;
		    ret = libfwsi_file_entry_extension_get_access_time(curblock, &accesstime, &itemerror);
		    uint8_t* adt = (uint8_t*)&accesstime;
		    uint16_t adate = (uint16_t)adt[0] | (uint16_t)adt[1] << 8;
		    uint16_t atime = (uint16_t)adt[2] | (uint16_t)adt[3] << 8;
		    filecontents->append("\tAccessed Time\t| " + ConvertDosTimeToHuman(&adate, &atime) + " UTC\n");
		    size_t lnamesize = 0;
		    ret = libfwsi_file_entry_extension_get_utf8_long_name_size(curblock, &lnamesize, &itemerror);
		    uint8_t* lname = new uint8_t[lnamesize+1];
		    ret = libfwsi_file_entry_extension_get_utf8_long_name(curblock, lname, lnamesize, &itemerror);
		    filecontents->append("\tLong Name\t| " + std::string((char*)lname) + "\n");
		    delete[] lname;
		    libfwsi_extension_block_free(&curblock, &itemerror);
                }
                filecontents->append("\n");
		ret = libfwsi_item_free(&curitem, &itemerror);
	    }

	    libfwsi_item_list_free(&itemlist, &itemerror);
	    libfwsi_error_free(&itemerror);
	    delete[] itemstream;
            //std::cout << "shell structure length: " << shellstructurelength << std::endl;
            curoffset = 76 + shellstructurelength + 2;
        }
        //std::cout << "curoffset: " << curoffset << std::endl;
        if(flags & 0x02)
        {
            // GET structure offsets
            ReadInteger(tmpbuf, curoffset, &totalstructurelength);
            //std::cout << "total structure length: " << totalstructurelength << std::endl;
            uint32_t nextoffset = 0;
            ReadInteger(tmpbuf, curoffset + 4, &nextoffset);
            //std::cout << "next offset after this: " << nextoffset << std::endl;
            uint32_t fileflags = 0;
            ReadInteger(tmpbuf, curoffset + 8, &fileflags);
            //std::cout << "file flags: " << fileflags << std::endl;
            std::bitset<8> volflagbits(fileflags);
            //std::cout  << "volflagbits: " << volflagbits << std::endl;
            if(volflagbits[0] == 1)
            {
                ReadInteger(tmpbuf, curoffset + 12, &voloffset);
                ReadInteger(tmpbuf, curoffset + 16, &basepathoffset);
                //std::cout << "voloffset: " << voloffset << std::endl;
                //std::cout << " basepathoffset: " << basepathoffset << std::endl;
                ReadInteger(tmpbuf, curoffset + voloffset, &volstructurelength);
                ReadInteger(tmpbuf, curoffset + voloffset + 4, &voltype);
                filecontents->append("Volume Type\t\t| ");
                if(voltype == 0)
                    filecontents->append("Unknown (0)");
                else if(voltype == 1)
                    filecontents->append("No Root Directory (1)");
                else if(voltype == 2)
                    filecontents->append("Removable (2)");
                else if(voltype == 3)
                    filecontents->append("Fixed (3)");
                else if(voltype == 4)
                    filecontents->append("Remote (4)");
                else if(voltype == 5)
                    filecontents->append("Optical Disc (5)");
                else if(voltype == 6)
                    filecontents->append("RAM Drive (6)");
                filecontents->append("\n");
                //std::cout << "voltype: " << voltype << std::endl;
                ReadInteger(tmpbuf, curoffset + voloffset + 8, &volserial);
                std::stringstream serialstream;
                serialstream << std::hex << volserial;
                filecontents->append("Volume Serial Number\t| 0x" + serialstream.str() + "\n");
                //std::cout << "vol serial: 0x" << std::hex << volserial << std::dec << std::endl;
                ReadInteger(tmpbuf, curoffset + voloffset + 12, &volnameoffset);
                //std::cout << "name length: " << volstructurelength << " " << volnameoffset << std::endl;
                //std::cout << "vol name length: " << volstructurelength - volnameoffset << std::endl;
                uint8_t* volname = new uint8_t[volstructurelength - volnameoffset +1];
                volname = substr(tmpbuf, curoffset + voloffset + volnameoffset, volstructurelength - volnameoffset);
                volname[volstructurelength - volnameoffset] = 0;
                //std::cout << "volname: " << (char*)volname << std::endl;
                volnamestr = std::string((char*)volname);
                filecontents->append("Volume Label\t\t| " + volnamestr + "\n");
                delete[] volname;
                //std::cout << "basepath global offset: " << curoffset + basepathoffset << std::endl;
                //std::cout << "next offset: " << curoffset + voloffset + volnameoffset + volstructurelength - volnameoffset + 1 << std::endl;
                //std::cout << "basepath length: " << totalstructurelength - basepathoffset - 1 << std::endl;
                uint8_t* basepathname = new uint8_t[totalstructurelength - basepathoffset];
                basepathname = substr(tmpbuf, curoffset + basepathoffset, totalstructurelength - basepathoffset - 1);
                basepathname[totalstructurelength - basepathoffset] = 0;
                basepathstr = std::string((char*)basepathname);
                filecontents->append("Local Path\t\t| " + basepathstr + "\n");
                delete[] basepathname;
                //std::cout << "base path name: " << (char*)basepathname << std::endl;
            }
            if(volflagbits[1] == 1)
            {
                ReadInteger(tmpbuf, curoffset + 20, &networkvoloffset);
                // PARSE NETWORK VOLUME STRUCTURE HERE
                //std::cout << "networkvoloffset: " << networkvoloffset << std::endl;
            }
            ReadInteger(tmpbuf, curoffset + 24, &remainingpathoffset);
            // PARSE REMAINING/FINAL PATH HERE
            //std::cout << "remainingpathoffset: " << remainingpathoffset << std::endl;
            //std::cout << "file or directory" << std::endl;
        }
        //std::cout << "on to different strings: " << curoffset + totalstructurelength << std::endl;
        curoffset = curoffset + totalstructurelength;
        if(flags & 0x04)
        {
            //std::cout << "has a descriptive string" << std::endl;
            uint16_t desclength = 0;
            ReadInteger(tmpbuf, curoffset, &desclength);
            //std::cout << "desclength: " << desclength << std::endl;
            if(flags & 0x80) // utf-16 unicode
            {
                for(int i=0; i < desclength; i++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(tmpbuf, curoffset + 2 + i*2, &singleletter);
                    descstring += (char)singleletter;
                }
                curoffset = curoffset + desclength * 2 + 2;
            }
            else // ascii
            {
                uint8_t* description = new uint8_t[desclength+1];
                description = substr(tmpbuf, curoffset + 2, desclength);
                description[desclength] = 0;
                descstring = std::string((char*)description);
                //std::cout << "description: " << (char*)description << std::endl;
                delete[] description;
                curoffset = curoffset + desclength + 2;
            }
            filecontents->append("Description\t\t| " + descstring + "\n");
            //std::cout << "description: " << descstring << std::endl;
        }
        if(flags & 0x08)
        {
            //std::cout << "has a relative path string" << std::endl;
            uint16_t relpathlength = 0;
            ReadInteger(tmpbuf, curoffset, &relpathlength);
            //std::cout << "relpathlength: " << relpathlength << std::endl;
            if(flags & 0x80)
            {
                for(int i=0; i < relpathlength; i++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(tmpbuf, curoffset + 2 + i*2, &singleletter);
                    relpathstr += (char)singleletter;
                }
                curoffset = curoffset + relpathlength * 2 + 2;
            }
            else
            {
                uint8_t* relpath = new uint8_t[relpathlength+1];
                relpath = substr(tmpbuf, curoffset + 2, relpathlength);
                relpath[relpathlength] = 0;
                relpathstr = std::string((char*)relpath);
                //std::cout << "relpath: " << relpath << std::endl;
                delete[] relpath;
                curoffset = curoffset + relpathlength + 2;
            }
            filecontents->append("Relative Path\t| " + relpathstr + "\n");
        }
        if(flags & 0x10)
        {
            //std::cout << "has a working directory" << std::endl;
            uint16_t workdirlength = 0;
            ReadInteger(tmpbuf, curoffset, &workdirlength);
            if(flags & 0x80)
            {
                for(int i=0; i < workdirlength; i++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(tmpbuf, curoffset + 2 + i*2, &singleletter);
                    workingdirectory += (char)singleletter;
                }
                curoffset = curoffset + workdirlength * 2 + 2;
            }
            else
            {
                uint8_t* workdirchar = new uint8_t[workdirlength+1];
                workdirchar = substr(tmpbuf, curoffset + 2, workdirlength);
                workdirchar[workdirlength] = 0;
                workingdirectory = std::string((char*)workdirchar);
                delete[] workdirchar;
                curoffset = curoffset + workdirlength + 2;
            }
            filecontents->append("Working Directory\t| " + workingdirectory + "\n");
            //std::cout << "working directory: " << workingdirectory << std::endl;
        }
        if(flags & 0x20)
        {
            //std::cout << "has command line arguments" << std::endl;
            uint16_t cmdlength = 0;
            ReadInteger(tmpbuf, curoffset, &cmdlength);
            if(flags & 0x80) // UTF-16 UNICODE
            {
                for(int i=0; i < cmdlength; i++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(tmpbuf, curoffset + 2 + i*2, &singleletter);
                    commandstring += (char)singleletter;
                }
                curoffset = curoffset + cmdlength * 2 + 2;
            }
            else // ASCII
            {
                uint8_t* cmdchar = new uint8_t[cmdlength+1];
                cmdchar = substr(tmpbuf, curoffset + 2, cmdlength);
                cmdchar[cmdlength] = 0;
                commandstring = std::string((char*)cmdchar);
                delete[] cmdchar;
                curoffset = curoffset + cmdlength + 2;
            }
            filecontents->append("Command Line\t| " + commandstring + "\n");
            //std::cout << "cmnd string: " << commandstring << std::endl;
        }
        if(flags & 0x40)
        {
            //std::cout << "has a custom icon" << std::endl;
            uint16_t iconlength = 0;
            ReadInteger(tmpbuf, curoffset, &iconlength);
            if(flags & 0x80) // UTF-16 UNICODE
            {
                for(int i=0; i < iconlength; i++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(tmpbuf, curoffset + 2 + i*2, &singleletter);
                    iconstring += (char)singleletter;
                }
                curoffset = curoffset + iconlength * 2 + 2;
            }
            else // ASCII
            {
                uint8_t* iconchar = new uint8_t[iconlength + 1];
                iconchar = substr(tmpbuf, curoffset + 2, iconlength);
                iconchar[iconlength] = 0;
                iconstring = std::string((char*)iconchar);
                delete[] iconchar;
                curoffset = curoffset + iconlength + 2;
            }
            filecontents->append("Icon File\t\t| " + iconstring + "\n");
            //std::cout << "icon string: " << iconstring << std::endl;
        }
	filecontents->append("\n");
        //std::cout << "curoffset after strings: " << curoffset << std::endl;
	//std::cout << "flags: " << flags << std::endl;
        // PARSE EXTRA BLOCKS HERE
        // THIS WORKS, NEED TO GET DISTRIBUTED TRACKING LINK BIT TO GET MACHINE IDENTIFIER
	int k = 1;
	while(curoffset < curfileitem->size)
	{
	    uint32_t blksize = 0;
	    ReadInteger(tmpbuf, curoffset, &blksize);
	    if(blksize == 0)
		break;
	    filecontents->append("Data Block\t\t| " + std::to_string(k) + "\n");
	    uint32_t blksig = 0;
	    ReadInteger(tmpbuf, curoffset + 4, &blksig);
	    if(blksig == 0xa0000001) // environment variables location (788 bytes)
	    {
		filecontents->append("\tSignature\t| Environment Variables Location (0xa0000001)\n");
		uint8_t* evloc = new uint8_t[260];
		evloc = substr(tmpbuf, curoffset + 8, 260);
		filecontents->append("\tENV VAR Location\t| " + std::string((char*)evloc) + "\n");
		delete[] evloc;
		k++;
		curoffset = curoffset + 788;
	    }
	    else if(blksig == 0xa0000002) // console properties (204 bytes)
	    {
		filecontents->append("\tSignature\t| Console Properties (0xa0000002)\n");
		k++;
		curoffset = curoffset + 204;
	    }
	    else if(blksig == 0xa0000003) // distributed link tracker properties (96 bytes)
	    {
		filecontents->append("\tSignature\t| Distributed Link Tracker Properties (0xa0000003)\n");
		uint8_t* machineid = new uint8_t[16];
		machineid = substr(tmpbuf, curoffset + 16, 16);
		filecontents->append("\tMachine ID\t| " + std::string((char*)machineid) + "\n");
		delete[] machineid;
		uint8_t* dvid = new uint8_t[16];
		dvid = substr(tmpbuf, curoffset + 32, 16);
		filecontents->append("\tDroid Volume ID | " + ReturnFormattedGuid(dvid) + "\n");
		delete[] dvid;
		uint8_t* fid = new uint8_t[16];
		fid = substr(tmpbuf, curoffset + 48, 16);
		filecontents->append("\tDroid File ID\t| " + ReturnFormattedGuid(fid) + "\n");
		delete[] fid;
		uint8_t* bdvid = new uint8_t[16];
		bdvid = substr(tmpbuf, curoffset + 64, 16);
		filecontents->append("\tBirth Volume ID | " + ReturnFormattedGuid(bdvid) + "\n");
		delete[] bdvid;
		uint8_t* bfid = new uint8_t[16];
		bfid = substr(tmpbuf, curoffset + 80, 16);
		filecontents->append("\tBirth File ID\t| " + ReturnFormattedGuid(bfid) + "\n");
		delete[] bfid;
		k++;
		curoffset = curoffset + 96;
	    }
	    else if(blksig == 0xa0000004) // console codepage (12 bytes)
	    {
		filecontents->append("\tSignature\t| Console Codepage (0xa0000004)\n");
		k++;
		curoffset = curoffset + 12;
	    }
	    else if(blksig == 0xa0000005) // special folder location (16 bytes)
	    {
		filecontents->append("\tSignature\t| Special Folder Location (0xa0000005)\n");
		k++;
		curoffset = curoffset + 16;
	    }
	    else if(blksig == 0xa0000006) // darwin properties (788 bytes)
	    {
		filecontents->append("\tSignature\t| Darwin Properties (0xa0000006)\n");
		uint8_t* dp = new uint8_t[260];
		dp = substr(tmpbuf, curoffset + 8, 260);
		filecontents->append("\tApp ID\t| " + std::string((char*)dp) + "\n");
		delete[] dp;
		k++;
		curoffset = curoffset + 788;
	    }
	    else if(blksig == 0xa0000007) // icon location (788 bytes)
	    {
		filecontents->append("\tSignature\t| Icon Location (0xa0000007)\n");
		uint8_t* dp = new uint8_t[260];
		dp = substr(tmpbuf, curoffset + 8, 260);
		filecontents->append("\tIcon Location\t| " + std::string((char*)dp) + "\n");
		delete[] dp;
		k++;
		curoffset = curoffset + 788;
	    }
	    else if(blksig == 0xa0000008) // shim layer properties (variable)
	    {
		filecontents->append("\tSignature\t| Shim Layer Properties (0xa0000008)\n");
		k++;
		curoffset = curoffset + blksize;
	    }
	    else if(blksig == 0xa0000009) // metadata property store (variable)
	    {
		filecontents->append("\tSignature\t| Metadata Property Store (0xa0000009)\n");
		k++;
		curoffset = curoffset + blksize;
	    }
	    else if(blksig == 0xa000000b) // known folder location (28 bytes)
	    {
		filecontents->append("\tSignature\t| Known Folder Location (0xa000000b)\n");
		k++;
		curoffset = curoffset + 28;
	    }
	    else if(blksig == 0xa000000c) // shell item identifiers list (variable)
	    {
		filecontents->append("\tSignature\t| Shell Item Identifiers List (0xa000000c)\n");
		k++;
		curoffset = curoffset + blksize;
	    }
	}
    }
    else if(curfileitem->sig.compare("Recycler") == 0) // INFO2 file
    {
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }
        std::string titlestring = "INFO2 File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
        filecontents->clear();
        filecontents->append(titlestring + "\n");
        for(int i=0; i < titlestring.size(); i++)
            filecontents->append("-");
        filecontents->append("\n\n");
        uint32_t fileentrysize = 0;
        ReadInteger(tmpbuf, 12, &fileentrysize);
        int curpos = 20;
        while(curpos < curfileitem->size)
        {
            uint8_t* fnamestr = new uint8_t[260];
            fnamestr = substr(tmpbuf, curpos, 260);
            uint32_t recycleindex = 0;
            ReadInteger(tmpbuf, curpos + 260, &recycleindex);
            uint32_t drivenumber = 0;
            ReadInteger(tmpbuf, curpos + 264, &drivenumber);
            filecontents->append("Index\t\t| " + std::to_string(recycleindex) + "\n");
            filecontents->append("File Path\t| " + std::string((char*)fnamestr) + "\n");
            delete[] fnamestr;
            std::string filenamestring = "";
            uint64_t deletedate = 0;
            ReadInteger(tmpbuf, curpos + 268, &deletedate);
            filecontents->append("Deleted Date\t| " + ConvertWindowsTimeToUnixTimeUTC(deletedate) + "\n");
            uint32_t fsize = 0;
            ReadInteger(tmpbuf, curpos + 276, &fsize);
            filecontents->append("File Size\t| " + ReturnFormattingSize(fsize) + " bytes\n\n");
            curpos = curpos + fileentrysize;
        }
    }
    else if(curfileitem->sig.compare("Recycle.Bin") == 0) // $I file
    {
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }
        std::string titlestring = "$I File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
        filecontents->clear();
        filecontents->append(titlestring + "\n");
        for(int i=0; i < titlestring.size(); i++)
            filecontents->append("-");
        filecontents->append("\n\n");
        uint64_t versionformat = 0;
        ReadInteger(tmpbuf, 0, &versionformat);
        uint64_t filesize = 0;
        ReadInteger(tmpbuf, 8, &filesize);
        uint64_t deletedate = 0;
        ReadInteger(tmpbuf, 16, &deletedate);
        uint32_t fnamesize = 0;
        uint32_t offset = 24;
        if(versionformat == 0x01)
            fnamesize = 520;
        else if(versionformat == 0x02)
        {
            uint32_t fnamesize = 0;
            ReadInteger(tmpbuf, 24, &fnamesize);
            offset = 28;
        }
        uint8_t* fnamestr = new uint8_t[fnamesize/2 +1];
        for(int i=0; i < fnamesize / 2; i++)
        {
            uint16_t singleletter = 0;
            ReadInteger(tmpbuf, offset + i*2, &singleletter);
            fnamestr[i] = (uint8_t)singleletter;
        }
        fnamestr[fnamesize] = 0;
        filecontents->append("File Path\t| " + std::string((char*)fnamestr) + "\n");
        filecontents->append("Deleted Date\t| " + ConvertWindowsTimeToUnixTimeUTC(deletedate) + "\n");
        filecontents->append("File Size\t| " + ReturnFormattingSize(filesize) + " bytes\n");
        delete[] fnamestr;
    }
    else if(curfileitem->sig.compare("Prefetch") == 0) // pf file
    {
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }

	uint8_t* udata = NULL;
        std::string titlestring = "Prefetch File Analysis for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
        filecontents->clear();
        filecontents->append(titlestring + "\n");
        for(int i=0; i < titlestring.size(); i++)
            filecontents->append("-");
        filecontents->append("\n\n");
	uint8_t* mamchar = new uint8_t[3];
	mamchar = substr(tmpbuf, 0, 3);
	if(std::string((char*)mamchar).compare("MAM") == 0) // WIN10+ Prefetch
	{
	    uint32_t datasize = 0;
	    ReadInteger(tmpbuf, 4, &datasize);
	    libfwnt_error_t* fwnterror = NULL;
	    size_t compressedsize = curfileitem->size - 8;
	    size_t usize = datasize;
	    uint8_t* compresseddata = new uint8_t[compressedsize];
	    udata = new uint8_t[usize];
	    compresseddata = substr(tmpbuf, 8, compressedsize);
	    libfwnt_lzxpress_huffman_decompress(compresseddata, compressedsize, udata, &usize, &fwnterror);
	    delete[] compresseddata;
	}
	else
	    udata = tmpbuf;
	delete[] mamchar;
	// NOW WE HAVE THE CONTENT FROM EITHER TYPE AS REGULAR PREFETCH, RUN SIG CHECK AND PARSE
	uint8_t* sigchar = new uint8_t[4];
	sigchar = substr(udata, 4, 4);
	if(std::string((char*)sigchar).compare("SCCA") == 0) // PARSE Prefetch
	{
            uint32_t pfversion = 0;
            ReadInteger(udata, 0, &pfversion);
            filecontents->append("Format Version\t\t| " + std::to_string(pfversion) + "\n");
            filecontents->append("Executable Filename\t| ");
            std::string fname = "";
            for(int i=0; i < 60; i++)
            {
                uint16_t singleletter = 0;
                ReadInteger(udata, 16 + i*2, &singleletter);
                if(singleletter == 0x0000)
                    break;
                fname += (char)singleletter;
            }
            filecontents->append(fname + "\n");
            uint32_t pfhash = 0;
            ReadInteger(udata, 76, &pfhash);
            std::stringstream hashstream;
            hashstream << std::hex << pfhash;
            filecontents->append("Prefetch Hash\t\t| 0x" + hashstream.str() + "\n");
            uint32_t metricsoffset = 0;
            ReadInteger(udata, 84, &metricsoffset);
            uint16_t fileinformationsize = 0;
            if(pfversion == 17) // WINXP, WIN2003
            {
                fileinformationsize = 68;
            }
            else if(pfversion == 23) // WINVISTA, WIN7
            {
                fileinformationsize = 156;
            }
            else if(pfversion == 26 || (pfversion == 30 && metricsoffset == 0x130)) // WIN8.1, WIN10
            {
                fileinformationsize = 224;
            }
            else if(pfversion == 30 && metricsoffset == 0x128) // WIN10
            {
                fileinformationsize = 216;
            }
            uint32_t fnamestringoffset = 0;
            ReadInteger(udata, 100, &fnamestringoffset);
            uint32_t fnamestringsize = 0;
            ReadInteger(udata, 104, &fnamestringsize);
            uint32_t volinfooffset = 0;
            ReadInteger(udata, 108, &volinfooffset);
            uint32_t volinfocount = 0;
            ReadInteger(udata, 112, &volinfocount);
            uint32_t volinfosize = 0;
            ReadInteger(udata, 116, &volinfosize);
            uint32_t runcount = 0;
            if(pfversion == 17)
                ReadInteger(udata, 60, &runcount);
            else if(pfversion == 23)
                ReadInteger(udata, 68, &runcount);
            else if(pfversion == 26 || (pfversion == 30 && metricsoffset == 0x130))
                ReadInteger(udata, 124, &runcount);
            else if(pfversion == 30 && metricsoffset == 0x128)
                ReadInteger(udata, 116, &runcount);
            filecontents->append("Run Count\t\t| " + std::to_string(runcount) + "\n");
            uint64_t lastruntime = 0;
            if(pfversion == 17)
            {
                ReadInteger(udata, 120, &lastruntime);
                filecontents->append("Last Run Time\t\t| " + ConvertWindowsTimeToUnixTimeUTC(lastruntime) + "\n");
            }
            else if(pfversion == 23)
            {
                ReadInteger(udata, 128, &lastruntime);
                filecontents->append("Last Run Time\t\t| " + ConvertWindowsTimeToUnixTimeUTC(lastruntime) + "\n");
            }
            else if(pfversion == 26 || pfversion == 30)
            {
                for(int i=0; i < 8; i++)
                {
                    ReadInteger(udata, 128 + i*8, &lastruntime);
                    if(lastruntime == 0)
                        filecontents->append("Last Run Time " + std::to_string(i+1) + "\t| Not Set (0)\n");
                    else
                        filecontents->append("Last Run Time " + std::to_string(i+1) + "\t| " + ConvertWindowsTimeToUnixTimeUTC(lastruntime) + "\n");
                }
            }
            std::string tmpstr = "";
            int increment = 1;
            //std::cout << "fname string offset: " << fnamestringoffset << " fname string size: " << fnamestringsize << std::endl;
            for(int i=0; i < fnamestringsize/2; i++)
            {
                uint16_t singleletter = 0;
                ReadInteger(udata, fnamestringoffset + i*2, &singleletter);
                if(singleletter == 0x0000)
                {
                    filecontents->append("File Name " + std::to_string(increment) + "\t\t| " + tmpstr + "\n");
                    increment++;
                    tmpstr = "";
                }
                else
                    tmpstr += (char)singleletter;
            }
            filecontents->append("\n");
            for(int i=0; i < volinfocount; i++)
            {
                int curpos = 0;
                uint32_t volpathoffset = 0;
                uint32_t volpathsize = 0;
                std::string volpathstr = "";
                if(pfversion == 17)
                    curpos = 40*i;
                else if(pfversion == 23 || pfversion == 26)
                    curpos = 104*i;
                else if(pfversion == 30)
                    curpos = 96*i;
                ReadInteger(udata, volinfooffset + curpos, &volpathoffset);
                ReadInteger(udata, volinfooffset + curpos + 4, &volpathsize);
                for(int j=0; j < volpathsize; j++)
                {
                    uint16_t singleletter = 0;
                    ReadInteger(udata, volinfooffset + curpos + volpathoffset + j*2, &singleletter);
                    volpathstr += (char)singleletter;
                }
                filecontents->append("Volume " + std::to_string(i+1) + " Path\t\t| " + volpathstr + "\n");
                uint64_t volctime = 0;
                ReadInteger(udata, volinfooffset + curpos + 8, &volctime);
                filecontents->append("Volume " + std::to_string(i+1) + " Creation\t| " + ConvertWindowsTimeToUnixTimeUTC(volctime) + "\n");
                uint32_t volserial = 0;
                ReadInteger(udata, volinfooffset + curpos + 16, &volserial);
                std::stringstream vserstream;
                vserstream << std::hex << volserial;
                filecontents->append("Volume " + std::to_string(i+1) + " Serial\t\t| 0x" + vserstream.str() + "\n");
            }
	}
    }
    else if(curfileitem->sig.compare("Directory") == 0)
    {
        std::vector<FileItem> fileitemvector;
        fileitemvector.clear();
        int filecount = 0;
        if(curfileitem->gid == 0)
            filecount = ReadDirectory(curitem, &fileitemvector, NULL);
        else
            filecount = ReadDirectory(curitem, &fileitemvector, curfileitem);
        filecontents->clear();
        std::string titlestring = "File Listing for " + curfileitem->name + " (" + std::to_string(curfileitem->gid) + ")";
        filecontents->append(titlestring + "\n");
        for(int i=0; i < titlestring.size(); i++)
            filecontents->append("-");
        filecontents->append("\n\n");
        /*
        std::string thstring = "#\tFileName\tSize";
        filecontents->append(thstring + "\n");
        for(int i=0; i < thstring.size() + 13; i++)
            filecontents->append("-");
        filecontents->append("\n");
        */
	for(int i=0; i < fileitemvector.size(); i++)
	    filecontents->append("(" + std::to_string(i+1) + ")  " + fileitemvector.at(i).name + "\n");
    }
    else if(curfileitem->sig.compare("Pdf") == 0)
    {
        /**/
        if(!inmemory)
        {
            std::ifstream file(tmpfilestr.c_str(),  std::ios::binary | std::ios::ate);
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            tmpbuf = new uint8_t[size];
            file.read((char*)tmpbuf, size);
        }
	poppler::document* pdfdoc;
	poppler::page* pdfpage;
	pdfdoc = poppler::document::load_from_raw_data((char*)tmpbuf, curfileitem->size);
	int pagecount = pdfdoc->pages();
	for(int i=0; i < pagecount; i++)
	    filecontents->append(pdfdoc->create_page(i)->text().to_latin1());
        /**/
    }
    else if(curfileitem->sig.compare("Html") == 0)
    {
	lxb_status_t status;
	lxb_html_tokenizer_t* tkz;

	tkz = lxb_html_tokenizer_create();
	status = lxb_html_tokenizer_init(tkz);
	lxb_html_tokenizer_callback_token_done_set(tkz, token_callback, filecontents);

	status = lxb_html_tokenizer_begin(tkz);
	status = lxb_html_tokenizer_chunk(tkz, tmpbuf, curfileitem->size);
	status = lxb_html_tokenizer_end(tkz);
	lxb_html_tokenizer_destroy(tkz);
	/*
	char htmlbuf[curfileitem->size + 1];
	memcpy(&htmlbuf, (char*)tmpbuf, curfileitem->size);
	htmlbuf[curfileitem->size] = 0;
	rapidxml::xml_document<> htmldoc;
	htmldoc.parse<0>(htmlbuf);
	//htmldoc.parse<0>(((char*)tmpbuf));
	rapidxml::xml_node<>* rootnode = htmldoc.first_node();
	GetXmlText(rootnode, filecontents);
	*/
	/*
        hubbub_parser* parser;
        hubbub_parser_optparams params;
        hubbub_tree_handler treehandler;
        hubbub_error huberr;
        huberr = hubbub_parser_create(NULL, false, &parser);
        huberr = hubbub_parser_parse_chunk(parser, tmpbuf, curfileitem->size);
        huberr = hubbub_parser_completed(parser);
	*/
        //params.document_node;
        //std::cout << "parse html and get text from it here..." << std::endl;
	/*
        huberr = hubbub_parser_destroy(parser);
	*/
    }
    else
    {
        // need to display hex here for the file...
        //std::cout << " launch internal/external viewer for files here..." << std::endl;
    }
}


