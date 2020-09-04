#include "imagefunctions.h"

unsigned long long GetTotalBytes(std::string instr)
{
    unsigned long long totbyt = 0;
    //printf("INFILE: %s\n", instr.c_str());
    int infile = open(instr.c_str(), O_RDONLY);
    ioctl(infile, BLKGETSIZE64, &totbyt);
    close(infile);
    //printf("total bytes: %lld\n", totbyt);
    return totbyt;
}

void ReadBytes(std::string instr, std::string outstr)
{
    unsigned long long totalbytes = 0;
    unsigned long long curpos = 0;
    int infile = open(instr.c_str(), O_RDONLY);
    int outfile = open(outstr.c_str(), O_APPEND);
    ioctl(infile, BLKGETSIZE64, &totalbytes);
    lseek(infile, 0, SEEK_SET);
    while (curpos <= 1024)
    {
        char bytebuf[512];
	ssize_t bytesread = read(infile, bytebuf, 512);
	ssize_t byteswrite = write(outfile, bytebuf, 512);
	curpos = curpos + 512;
    }
    //printf("Bytes: %c %c %c\n", bytebuf[0], bytebuf[1], bytebuf[2]);
    close(infile);
    close(outfile);
}
