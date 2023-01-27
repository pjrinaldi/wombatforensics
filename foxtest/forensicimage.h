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
};

#endif
