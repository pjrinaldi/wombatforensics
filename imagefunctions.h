#include <fcntl.h>
#include <linux/fs.h>
#include <linux/hdreg.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <openssl/md5.h>
#include <openssl/evp.h>
#include <cstring>
#include <sstream>
#include <fstream>
//#include <QTextStream>

#define DTTMFMT "%F %T %z"
#define DTTMSZ 35

static char* GetDateTime(char *buff)
{
    time_t t = time(0);
    strftime(buff, DTTMSZ, DTTMFMT, localtime(&t));
    return buff;
};

unsigned long long GetTotalBytes(std::string infile);
void ReadBytes(std::string infile, std::string outfile);
//void Verify(std::string instr, std::string outstr);
std::string Verify(std::string outstr);
//std::string Verify(std::string instr);
void StartImaging(std::string instr, std::string outpath, std::string outstr, int radio); 
