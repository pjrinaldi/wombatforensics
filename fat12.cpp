#include "fat12.h"

void LoadFatDirectory(CurrentItem* currentitem, std::vector<FileItem>* filevector, FileItem* curfileitem)
{
    //if(curfileitem != NULL)
    //    std::cout << "curfileitem name: " << curfileitem->name << std::endl;
    // BYTES PER SECTOR
    uint16_t bytespersector = 0;
    ReadForImgContent(currentitem->forimg, &bytespersector, currentitem->voloffset + 11);
    //std::cout << "bytes per sector: " << bytespersector << std::endl;
    // FAT COUNT
    uint8_t fatcount = 0;
    currentitem->forimg->ReadContent(&fatcount, currentitem->voloffset + 16, 1);
    //std::cout << "fat count: " << (uint)fatcount << std::endl;
    // SECTORS PER CLUSTER
    uint8_t sectorspercluster = 0;
    currentitem->forimg->ReadContent(&sectorspercluster, currentitem->voloffset + 13, 1);
    //std::cout << "sectors per cluster: " << (uint) sectorspercluster << std::endl;
    // RESERVED AREA SIZE
    uint16_t reservedareasize = 0;
    ReadForImgContent(currentitem->forimg, &reservedareasize, currentitem->voloffset + 14);
    //std::cout << "reserved area size: " << reservedareasize << std::endl;
    // ROOT DIRECTORY MAX FILES
    uint16_t rootdirmaxfiles = 0;
    ReadForImgContent(currentitem->forimg, &rootdirmaxfiles, currentitem->voloffset + 17);
    //std::cout << "root dir max files: " << rootdirmaxfiles << std::endl;
    if(currentitem->itemtext.find("[FAT12]") != std::string::npos || currentitem->itemtext.find("[FAT16]") != std::string::npos)
    {
        // FAT SIZE
        uint16_t fatsize = 0;
        ReadForImgContent(currentitem->forimg, &fatsize, currentitem->voloffset + 22);
        //std::cout << "fat size: " << fatsize << std::endl;
        // CLUSTER AREA START
        uint64_t clusterareastart = reservedareasize + fatcount * fatsize + ((rootdirmaxfiles * 32) + (bytespersector - 1)) / bytespersector;
        //std::cout << "Cluster area start: " << clusterareastart << std::endl;
        // DIRECTORY OFFSET
        uint64_t diroffset = (reservedareasize + fatcount * fatsize) * bytespersector;
        //std::cout << "dir offset: " << diroffset << std::endl;
        // DIRECTORY SIZE
        uint64_t dirsize = rootdirmaxfiles * 32 + bytespersector - 1;
        //std::cout << "dir size: " << dirsize << std::endl;
        std::string curdirlayout = "";
        // ROOT DIRECTORY LAYOUT
        std::string rootdirlayout = std::to_string(diroffset) + "," + std::to_string(dirsize) + ";";
        //std::cout << "root dir layout: " << rootdirlayout << std::endl;
        //if(curinode == 0) // root directory
        if(curfileitem == NULL) // root directory
        {
            curdirlayout = rootdirlayout;
        }
        else // sub directory
        {
            curdirlayout = curfileitem->layout;
        }
        //std::cout << "cur dir layout: " << curdirlayout << std::endl;
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
            uint direntrycount = dirsize / 32;
            //std::cout << "dir entry count: " << direntrycount << std::endl;
	    // PARSE DIRECTORY ENTRIES
            std::string longnamestring = "";
            for(uint i=0; i < direntrycount; i++)
            {
                FileItem tmpitem;
                //std::string filename = "";
                //uint32_t clusternum = 0;
                //uint64_t physicalsize = 0;
	        // FIRST CHARACTER
                uint8_t firstchar = 0;
                currentitem->forimg->ReadContent(&firstchar, diroffset + i*32, 1);
                if((uint)firstchar == 0x00) // entry is free and all remaining are free
                    break;
                else
                {
                    uint8_t fileattr = 0;
                    currentitem->forimg->ReadContent(&fileattr, diroffset + i*32 + 11, 1);
                    //std::cout << "file attr: " << std::hex << (uint)fileattr << std::endl;
                    //std::cout << "first char: " << std::hex << (uint)firstchar << std::endl;
                    if((uint)fileattr == 0x0f || (uint)fileattr == 0x3f) // long directory name
                    {
                        uint lsn = ((int)firstchar & 0x0f);
                        if(lsn <= 20)
                        {
                            std::string longname = "";
                            int arr[13] = {1, 3, 5, 7, 9, 14, 16, 18, 20, 22, 24, 28, 30};
                            for(int j=0; j < 13; j++)
                            {
                                uint16_t longletter = 0;
                                ReadForImgContent(currentitem->forimg, &longletter, diroffset + i*32 + arr[j]);
                                if(longletter < 0xFFFF)
                                    longname += (char)longletter;
                            }
                            longnamestring.insert(0, longname);
                        }
                    }
                    else
                    {
                        // FILE OR DIRECTORY
                        //bool isdirectory = false; // file
                        if((uint)fileattr & 0x10)
                            tmpitem.isdirectory = true; // directory
                        //std::cout << "is directory: " << isdirectory << std::endl;
                        // DELETED OR NOT DELETED
                        //bool isdeleted = false; // not deleted
                        if((uint)firstchar == 0x05 || (uint)firstchar == 0xe5)
                            tmpitem.isdeleted = true; // deleted
                        //std::cout << "is deleted: " << isdeleted << std::endl;
                        // FILENAME
                        //std::string filename = "";
                        if(!longnamestring.empty())
                        {
                            tmpitem.name = longnamestring;
                            //filename = longnamestring;
                        }
                        else
                        {
                            char* rname = new char[8];
                            currentitem->forimg->ReadContent((uint8_t*)rname, diroffset + i*32 + 1, 7);
                            rname[7] = 0;
                            std::string restname(rname);
                            char* ename = new char[4];
                            currentitem->forimg->ReadContent((uint8_t*)ename, diroffset + i*32 + 8, 3);
                            ename[3] = 0;
                            std::string extname(ename);
                            while(extname.size() > 0)
                            {
                                std::size_t findempty = extname.find(" ", 0);
                                if(findempty != std::string::npos)
                                    extname.erase(findempty, 1);
                                else
                                    break;
                            }
                            restname.erase(std::remove_if(restname.begin(), restname.end(), isspace), restname.end());
                            if(tmpitem.isdeleted)
                                tmpitem.name = std::string(1, '_') + restname;
                            else
                                tmpitem.name = std::string(1, (char)firstchar) + restname;
                            if(extname.size() > 0)
                                tmpitem.name += "." + extname;
                        }
                        tmpitem.path = "/";
                        //std::cout << "file name: " << filename << std::endl;
                        longnamestring = "";
                        // LOGICAL FILE SIZE
                        uint32_t logicalsize = 0;
                        ReadForImgContent(currentitem->forimg, &logicalsize, diroffset + i*32 + 28);
                        //std::cout << "logical file size: " << logicalsize << std::endl;
                        tmpitem.size = logicalsize;
                        // CREATE DATE
                        uint16_t createdate = 0;
                        ReadForImgContent(currentitem->forimg, &createdate, diroffset + i*32 + 16);
                        uint16_t createtime = 0;
                        ReadForImgContent(currentitem->forimg, &createtime, diroffset + i*32 + 14);
                        tmpitem.create = ConvertDosTimeToHuman(&createdate, &createtime);
                        //std::cout << "Created Date: " << datecreated << std::endl;
                        // ACCESS DATE
                        uint16_t accessdate = 0;
                        ReadForImgContent(currentitem->forimg, &accessdate, diroffset + i*32 + 18);
                        tmpitem.access = ConvertDosTimeToHuman(&accessdate, NULL);
                        //std::cout << "Accessed Date: " << dateaccessed << std::endl;
                        // MODIFY DATE
                        uint16_t modifydate = 0;
                        ReadForImgContent(currentitem->forimg, &modifydate, diroffset + i*32 + 24);
                        uint16_t modifytime = 0;
                        ReadForImgContent(currentitem->forimg, &modifytime, diroffset + i*32 + 22);
                        tmpitem.modify = ConvertDosTimeToHuman(&modifydate, &modifytime);
                        //std::cout << "Modified Date: " << datemodified << std::endl;
                        uint16_t hiclusternum = 0;
                        ReadForImgContent(currentitem->forimg, &hiclusternum, diroffset + i*32 + 20); // always 0 for FAT12/16
                        uint16_t loclusternum = 0;
                        ReadForImgContent(currentitem->forimg, &loclusternum, diroffset + i*32 + 26);
                        uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
                        //std::cout << "clusternum: " << clusternum << std::endl;
                        std::vector<uint> clusterlist;
                        clusterlist.clear();
                        if(clusternum >= 2)
                        {
                            //std::cout << "currentitem itemtext: " << currentitem->itemtext << std::endl;
                            clusterlist.push_back(clusternum);
                            if(currentitem->itemtext.find("[FAT12]") != std::string::npos)
                                GetNextCluster(currentitem->forimg, clusternum, 1, currentitem->voloffset + reservedareasize * bytespersector, &clusterlist);
                            else if(currentitem->itemtext.find("[FAT16]") != std::string::npos)
                                GetNextCluster(currentitem->forimg, clusternum, 2, currentitem->voloffset + reservedareasize * bytespersector, &clusterlist);
		            //GetNextCluster(curforimg, rootdircluster, 4, fatoffset, &clusterlist);
//void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, std::vector<uint>* clusterlist);
                            //QString::number((qulonglong)(curstartsector*512 + reservedareasize * bytespersector))
                            //currentitem->voloffset + reservedareasize * bytespersector
                        }
                        //for(int i=0; i < clusterlist.size(); i++)
                        //    std::cout << "cluster " << i << ": " << clusterlist.at(i) << std::endl;
                        if(clusterlist.size() > 0)
                            tmpitem.layout = ConvertBlocksToExtents(&clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
                        if(tmpitem.isdirectory)
                        {
                            tmpitem.cat = "Directory";
                            tmpitem.sig = "Directory";
                        }
                        else
                        {
                            GenerateCategorySignature(currentitem, &tmpitem.name, &(tmpitem.layout), &(tmpitem.cat), &(tmpitem.sig));
                            //std::cout << "cat/sig: " << tmpitem.cat << " " << tmpitem.sig << std::endl;
                        }
                        //std::cout << "tmpitem layout: " << tmpitem.layout << std::endl;
                        /*
                        uint16_t hiclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 20, 2)); // always zero for fat12/16
                        uint16_t loclusternum = qFromLittleEndian<uint16_t>(curimg->ReadContent(rootdiroffset + j*32 + 26, 2));
                        uint32_t clusternum = ((uint32_t)hiclusternum >> 16) + loclusternum;
                        QList<uint> clusterlist;
                        clusterlist.clear();
                        if(clusternum >= 2)
                        {
                            clusterlist.append(clusternum);
                            GetNextCluster(curimg, clusternum, fstype, fatoffset, &clusterlist);
                        }
                        QString layout = "";
                        if(clusterlist.count() > 0)
                            layout = ConvertBlocksToExtents(clusterlist, sectorspercluster * bytespersector, clusterareastart * bytespersector);
                        out << "Layout|" << layout << "|File offset,size; layout in bytes." << Qt::endl;
                        qulonglong physicalsize = clusterlist.count() * sectorspercluster * bytespersector;
                        out << "Physical Size|" << QString::number(physicalsize) << "|Sector Size in Bytes for the file." << Qt::endl;
                         */ 
                        if(!tmpitem.layout.empty())
                            filevector->push_back(tmpitem);
                    }
                }
            }
        }
        if(curfileitem == NULL) // root directory - add virtual files
        {
	    //curinode = AddVirtualFileSystemFiles(curimg, ptreecnt, fatcount, fatsize * bytespersector, curinode);
            FileItem tmpitem;
            tmpitem.size = bytespersector;
            tmpitem.name = "$MBR";
            tmpitem.path = "/";
            tmpitem.layout = std::to_string(currentitem->voloffset) + "," + std::to_string(bytespersector) + ";";
            tmpitem.isvirtual = true;
            tmpitem.cat = "System File";
            tmpitem.sig = "Master Boot Record";
            filevector->push_back(tmpitem);
            for(int i=0; i < fatcount; i++)
            {
                tmpitem.clear();
                tmpitem.size = fatsize * bytespersector;
                tmpitem.name = "$FAT" + std::to_string(i+1);
                tmpitem.path = "/";
                tmpitem.layout = std::to_string(currentitem->voloffset + reservedareasize * bytespersector + fatsize * i * bytespersector) + "," + std::to_string(fatsize * bytespersector) + ";";
                tmpitem.isvirtual = true;
                tmpitem.cat = "System File";
                tmpitem.sig = "File Allocation Table";
                filevector->push_back(tmpitem);
            }
        }
    }
    else // FAT32, EXFAT
    {
    }
}
