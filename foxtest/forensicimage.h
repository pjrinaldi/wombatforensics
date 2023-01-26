/*
class ForImg : public QIODevice
{
    public:
        ForImg(QString imgfile);
        ~ForImg();
        QByteArray ReadContent(qint64 pos, qint64 size);
        qint64 Size();
        QString ImgPath();
        QString MountPath();
        void SetMountPath(QString mountpath);
	int8_t ImgType();

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
        //int imgtype = 15;
        int8_t imgtype = -1;
        uint8_t hashtype = 0;
        int blake3hash = 0;
        QList<qint64> framelist;
};
*/
