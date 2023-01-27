#ifndef FORENSICIMAGE_H
#define FORENSICIMAGE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <filesystem>
// SHARED LIBRARIES
#include <libewf.h>
#include <libsmraw.h>
#include <aff4/aff4-c.h>
#include <zstd.h>

struct wfi_metadata
{
    uint32_t skipframeheader; // skippable frame header
    uint32_t skipframesize; // skippable frame content size (not including header and this size
    uint16_t sectorsize; // raw forensic image sector size
    int64_t reserved; // reserved
    int64_t totalbytes; // raw forensic image total size
    char casenumber[24]; // 24 character string
    char evidencenumber[24]; // 24 character string
    char examiner[24]; // 24 character string
    char description[128]; // 128 character string
    uint8_t devhash[32]; // blake3 source hash
};

class ForImg 
{
    public:
        ForImg(std::string imgfile);
        ~ForImg();
        void ReadContent(uint8_t* buf, uint64_t pos, uint64_t size);
        uint64_t Size();
        std::string ImagePath();
        //std::string MountPath();
        //void SetMountPath(std::string mountpath);
        int8_t ImgType();
    private:
        std::ifstream imagebuffer;
        //std::ifstream devicebuffer(devicestring.c_str(), std::ios::in|std::ios::binary);
        size64_t imgsize = 0;
        uint64_t imgoffset  = 0;
        std::string imgpath = "";
        //std::string mntpath = "";
        int8_t imgtype = -1;
	wfi_metadata wfimd;
};

#endif
