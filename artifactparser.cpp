#include "artifactparser.h"

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

void ParseArtifact(ForImg* curforimg, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents)
{
    std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
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
            //std::string tmpfilestr = "/tmp/wf/" + curfileitem->name + "-" + std::to_string(curfileitem->gid) + ".tmp";
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
        //if(inmemory)
        //{
            ReadInteger(tmpbuf, 20, &flags);
            //if(flags & 0x80)
            //    std::cout << "data strings are unicode rather than ascii." << std::endl;
            //std::cout << "flags: " << std::hex << flags << std::dec << std::endl;
            std::bitset<8> flagbits(flags); // values are reversed
            //std::cout << "flagbits: " << flagbits << std::endl;
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
            filecontents->append("File Size\t\t| " + ReturnFormattingSize(filesize) + " bytes\n");
            if(flagbits[0] == 1) // SHELL ITEM ID LIST IS PRESENT
            {
                ReadInteger(tmpbuf, 76, &shellstructurelength);
                //std::cout << "shell structure length: " << shellstructurelength << std::endl;
                // SKIP FOR NOW
                //std::cout << "shell id is present" << std::endl;
                curoffset = 76 + shellstructurelength + 2;
            }
            //std::cout << "curoffset: " << curoffset << std::endl;
            if(flagbits[1] == 1)
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
                    //std::cout << "networkvoloffset: " << networkvoloffset << std::endl;
                }
                ReadInteger(tmpbuf, curoffset + 24, &remainingpathoffset);
                //std::cout << "remainingpathoffset: " << remainingpathoffset << std::endl;
                //std::cout << "file or directory" << std::endl;
            }
            //std::cout << "on to different strings: " << curoffset + totalstructurelength << std::endl;
            curoffset = curoffset + totalstructurelength;
            if(flagbits[2] == 1)
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
            if(flagbits[3] == 1)
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
            if(flagbits[4] == 1)
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
            if(flagbits[5] == 1)
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
            if(flagbits[6] == 1)
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
            //std::cout << "curoffset after strings: " << curoffset << std::endl;
            // THIS WORKS, NEED TO GET DISTRIBUTED TRACKING LINK BIT TO GET MACHINE IDENTIFIER
        /*
        }
        else // this should never occur, i don't think LNK files are every going to be bigger than 4GB.
        {
            tmpfile = fopen(tmpfilestr.c_str(), "rb");
            fseek(tmpfile, 0x14, SEEK_SET);
            uint8_t* fbuf = new uint8_t[4];
            fread(fbuf, 1, 4, tmpfile);
            ReadInteger(fbuf, 0, &flags);
            delete[] fbuf;
            fclose(tmpfile);
        }
        */
        /*
                tmpuint32 = 0;
                liblnk_file_get_file_attribute_flags(lnkobj, &tmpuint32, &error);
                htmlstr += "<tr style='" + ReturnCssString(4) + "'><td style='" + ReturnCssString(9) + "'>File Attributes:</td><td style='" + ReturnCssString(15) + "'>0x" + QString("%1").arg(tmpuint32, 8, 16, QChar('0')) + "<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_READ_ONLY) != 0)
                    htmlstr += "Read Only (FILE_ATTRIBUTE_READ_ONLY)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_HIDDEN) != 0)
                    htmlstr += "Hidden (FILE_ATTRIBUTE_HIDDEN)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_SYSTEM) != 0)
                    htmlstr += "System (FILE_ATTRIBUTE_SYSTEM)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_DIRECTORY) != 0)
                    htmlstr += "Directory (FILE_ATTRIBUTE_DIRECTORY)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_ARCHIVE) != 0)
                    htmlstr += "Archived (FILE_ATTRIBUTE_ARCHIVE)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_DEVICE) != 0)
                    htmlstr += "Device (FILE_ATTRIBUTE_DEVICE)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_NORMAL) != 0)
                    htmlstr += "Normal (FILE_ATTRIBUTE_NORMAL)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_TEMPORARY) != 0)
                    htmlstr += "Temporary (FILE_ATTRIBUTE_TEMPORARY)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_SPARSE_FILE) != 0)
                    htmlstr += "Sparse File (FILE_ATTRIBUTE_SPARSE)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_REPARSE_POINT) != 0)
                    htmlstr += "Reparse Point (FILE_ATTRIBUTE_REPARSE_POINT)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_COMPRESSED) != 0)
                    htmlstr += "Compressed (FILE_ATTRIBUTE_COMPRESSED)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_OFFLINE) != 0)
                    htmlstr += "Offline (FILE_ATTRIBUTE_OFFLINE)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_NOT_CONTENT_INDEXED) != 0)
                    htmlstr += "Content should not be indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_ENCRYPTED) != 0)
                    htmlstr += "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)<br/>";
                if((tmpuint32 & LIBLNK_FILE_ATTRIBUTE_FLAG_VIRTUAL) != 0)
                    htmlstr += "Virtual (FILE_ATTRIBUTE_VIRTUAL)<br/>";
                htmlstr += "</td></tr>";
         */ 
    }
    else
        std::cout << "launch internal/external viewer for files here..." << std::endl;
}
