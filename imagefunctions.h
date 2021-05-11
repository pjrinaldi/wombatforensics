#include <fcntl.h>
#include <linux/fs.h>
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
#include <libewf.h>
#include <afflib/afflib.h>
#include <QString>
#include <QDebug>
#include <QDir>

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
std::string Verify(QString outstr);
void StartImaging(std::string instr, std::string outpath, std::string outstr, int radio); 

class ForImg
{
    public:
        ForImg(QString imgfile);
        ~ForImg();
        QByteArray ReadContent(qint64 pos, qint64 size);
        qint64 Size();
        QString ImgPath();

    private:

        // EWF Variables
        libewf_handle_t* ewfhandle = NULL;
        libewf_error_t* ewferror = NULL;
        char** globfiles = NULL;
        int globfilecnt = 0;
        // ALL Variables
        off64_t imgsize = 0;
        qint64 imgoffset = 0;
        QString imgpath = "";
        int imgtype = -1;
};
