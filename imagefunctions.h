#ifndef IMAGEFUNCTIONS_H
#define IMAGEFUNCTIONS_H

#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <stdio.h>
//#include <openssl/md5.h>
//#include <openssl/evp.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <libewf.h>
#include <afflib/afflib.h>
//#include <aff4-c.h>
//#include <libsmraw.h>
#include <QString>
#include <QDebug>
#include <QDir>
#include <QDataStream>
#include <QtEndian>

#include <blake3.h>
#include <lz4.h>
#include <lz4frame.h>

#define DTTMFMT "%F %T %z"
#define DTTMSZ 35

static char* GetDateTime(char *buff)
{
    time_t t = time(0);
    strftime(buff, DTTMSZ, DTTMFMT, localtime(&t));
    return buff;
};

void FindNextFrame(qint64 initialindex, QList<qint64>* framelist, QFile* wfi);
unsigned long long GetTotalBytes(std::string infile);
void ReadBytes(std::string infile, std::string outfile, std::string footstr);
std::string Verify(QString outstr);
void StartImaging(std::string instr, std::string outpath, std::string outstr, std::string footstr, int radio); 

class ForImg : public QIODevice
{
    Q_OBJECT
    public:
        ForImg(QString imgfile);
        ~ForImg();
        QByteArray ReadContent(qint64 pos, qint64 size);
        qint64 Size();
        QString ImgPath();
        QString MountPath();
        void SetMountPath(QString mountpath);

    protected:

	qint64 readData(char* data, qint64 maxsize)
	{
	    return maxsize;
	};
	qint64 writeData(const char* data, qint64 maxsize)
	{
	    return maxsize;
	};

    private:

        off64_t imgsize = 0;
        qint64 imgoffset = 0;
        QString imgpath = "";
        QString mntpath = "";
        int imgtype = -1;
        uint8_t hashtype = 0;
        int blake3hash = 0;
        QList<qint64> framelist;
};

/*
class MyIODevice : public QIODevice
{
    public:

        MyIODevice(QString imgfile);
        ~MyIODevice();
        qint64 readData(char *data, qint64 maxSize);
        qint64 writeData(char* data, qint64 maxSize);
    private:
        qint64 imgoffset = 0;
        QString imgpath = "";
        int imgtype = -1;
        qint64 imgsize = 0;
};
*/

#endif // IMAGEFUNCTIONS_H
