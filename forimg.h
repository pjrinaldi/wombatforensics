#include <libewf.h>
#include <afflib/afflib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <QIODevice>

#ifndef FORIMG_H
#define FORIMG_H

class ForensicImage : public QIODevice
{

public:

    ForensicImage(QString imgfile)
    {
        if(imgfile.split("/").last().toLower().endsWith(".e01"))
            imgtype = 0; // EWF
        else if(imgfile.split("/").last().toLower().endsWith(".aff"))
            imgtype = 1; // AFF
        else if(imgfile.split("/").last().toLower().endsWith(".001"))
            imgtype = 2; // split raw or raw
        else if(imgfile.split("/").last().toLower().endsWith(".zmg"))
            imgtype = 3; // ZMG
        imgpath = imgfile;

        QString efilepath = imgfile.split(imgfile.split("/").last()).first();
        QDir edir = QDir(imgfile.split(imgfile.split("/").last()).first());
        QStringList efiles = edir.entryList(QStringList() << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".e??") << QString(imgfile.split("/").last().toLower().split(".e01").first() + ".E??"), QDir::NoSymLinks | QDir::Files);
        char* filenames[efiles.count()] = {NULL};
        for(int i=0; i < efiles.count(); i++)
        {
            filenames[i] = QString(efilepath + efiles.at(i)).toLatin1().data();
            printf("filenames[%d] = %s\n", i, filenames[i]);
        }
        globfilecnt = efiles.count();
        printf("globfilecnt: %d\n", globfilecnt);
        int retopen = 0;

        retopen = libewf_glob(filenames[0], strlen(filenames[0]), LIBEWF_FORMAT_UNKNOWN, &globfiles, &globfilecnt, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        else
            printf("libewf glob was successful: %d\n", retopen);

        retopen = libewf_handle_initialize(&ewfhandle, &ewferror);
        if(retopen == -1)
            libewf_error_fprint(ewferror, stdout);
        libewf_handle_get_media_size(ewfhandle, (size64_t*)&imgsize, &ewferror);
    };

    ~ForensicImage()
    {
        if(imgtype == 0)
        {
            //libewf_handle_close(ewfhandle, &ewferror);
            libewf_handle_free(&ewfhandle, &ewferror);
            libewf_glob_free(globfiles, globfilecnt, &ewferror);
        }
    };

    qint64 readData(char *data, qint64 maxSize)
    {
        off64_t res = 0;
        if(imgtype == 0)
            res = libewf_handle_read_buffer(ewfhandle, data, maxSize, &ewferror);
        return res;
        /*
        int res = 0;
	(void) fi;
	if(strcmp(path, erawpath) != 0)
		return -ENOENT;
        res = libewf_handle_seek_offset(ewfhandle, offset, SEEK_SET, &ewferror);
	errno = 0;
        res = libewf_handle_read_buffer(ewfhandle, buf, size, &ewferror);
	return res;
        */
    };

    qint64 writeData(const char *data, qint64 maxSize)
    {
    };

    bool seek(qint64 pos)
    {
        QIODevice::seek(pos);
        if(imgtype == 0)
            imgoffset = libewf_handle_seek_offset(ewfhandle, pos, SEEK_SET, &ewferror);
        if(imgoffset == -1)
            return false;
        else
            return true;
    };

    qint64 size() const
    {
        //if(imgtype == 0)
        //    libewf_handle_get_media_size(ewfhandle, (size64_t*)&imgsize, &ewferror);
        qDebug() << "erawsize:" << imgsize;
        return imgsize;
    };

    QString ImgPath()
    {
        return imgpath;
    };

    QByteArray read(qint64 maxSize)
    {
        char* tdata = new char[maxSize];
        qint64 retval = readData(tdata, maxSize);
        if(retval > 0)
            return QByteArray::fromRawData((const char*)tdata, maxSize);
        //qint64 retval = testimage->readData(tdata, 512);
        //qDebug() << "retval:" << retval;
        //qDebug() << "1st sector:" << QByteArray::fromRawData((const char*)tdata, 512).toHex();
    };

    qint64 write(const char *data)
    {
    };
    
    bool open(QIODevice::OpenMode mode)
    {
        int retopen = 0;
        if(imgtype == 0)
        {
            retopen = libewf_handle_open(ewfhandle, globfiles, globfilecnt, LIBEWF_OPEN_READ, &ewferror);
            if(retopen == -1)
                libewf_error_fprint(ewferror, stdout);
            else
                printf("libewf_handle_open was successful %d\n", retopen);
        }
    };

    void close()
    {
        if(imgtype == 0)
            libewf_handle_close(ewfhandle, &ewferror);
    };

private:
    // EWF Variables
    libewf_handle_t* ewfhandle = NULL;
    libewf_error_t* ewferror = NULL;
    char** globfiles = NULL;
    int globfilecnt = 0;
    //off64_t erawsize = 0;
    //qint64 ewfoffset = 0;
    off64_t imgsize = 0;
    qint64 imgoffset = 0;
    QString imgpath = "";
    int imgtype = 0;
};

#endif // FORIMG_H

/*
#include "inputbuffer.h"

InputBuffer::InputBuffer(QString fileName)
{
    QFileInfo fileInfo(fileName);

    length   = fileInfo.size();
    position = 0;

    bufferRef = (char*)malloc(length);

    QFile file(fileName);
    file.open(QFile::ReadOnly);
    file.read(bufferRef, length);
    file.close();

    emit readyRead();
}


InputBuffer::~InputBuffer()
{
    free(bufferRef);
}


qint64 InputBuffer::readData(char *data, qint64 maxlen)
{
    if (position >= length) {
        return -1;
    }

    qint64 readSize = qMin(maxlen, length - position);

    memcpy(data, bufferRef + position, readSize);
    position += readSize;

    return readSize;
}


qint64 InputBuffer::writeData(const char *data, qint64 len)
{
    return -1;
}


bool InputBuffer::isSequential()
{
    return true;
}


qint64 InputBuffer::bytesAvailable()
{
    return length - position;
}


qint64 InputBuffer::size()
{
    return length;
}
*/

/*
 *inputBufferRef = new InputBuffer("/home/pp/Zenék/Test.mp3");
inputBufferRef->open(QIODevice::ReadOnly);

audioDecoderRef = new QAudioDecoder();

audioDecoderRef->setAudioFormat(audioFormat);
audioDecoderRef->setSourceDevice(inputBufferRef);

connect(audioDecoderRef, SIGNAL(bufferReady()),               this, SLOT(decoderBufferReady()));
connect(audioDecoderRef, SIGNAL(finished()),                  this, SLOT(decoderFinished()));
connect(audioDecoderRef, SIGNAL(error(QAudioDecoder::Error)), this, SLOT(decoderError(QAudioDecoder::Error)));

audioDecoderRef->start();
 */ 
