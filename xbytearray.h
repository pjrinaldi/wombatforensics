#ifndef XBYTEARRAY_H
#define XBYTEARRAY_H

/** \cond docNever */

#include <QtCore>
#include "tskvariable.h"

/*! XByteArray represents the content of QHexEcit.
XByteArray comprehend the data itself and informations to store if it was
changed. The QHexEdit component uses these informations to perform nice
rendering of the data

XByteArray also provides some functionality to insert, replace and remove
single chars and QByteArras. Additionally some functions support rendering
and converting to readable strings.
*/
class XByteArray
{
public:
    explicit XByteArray();

    int AddressOffset();
    void SetAddressOffset(int offset);

    int AddressWidth();
    void SetAddressWidth(int width);
    int BlockLineCount(void);
    int LineCount(void);

    QByteArray & data();
    void SetData(QByteArray data);
    bool OpenImage(TskObject* tskobject);
    bool CloseImage();
    size_t ReadImage(); // here is where i need to map the QFileDevice (as in # of pages)
    bool LoadSlice();
    void FreeSlice();
    off_t SeekImage();

    /*
    bool dataChanged(int i);
    QByteArray dataChanged(int i, int len);
    void setDataChanged(int i, bool state);
    void setDataChanged(int i, const QByteArray & state);
    */
    int RealAddressNumbers();
    int size();

    /*
    QByteArray & insert(int i, char ch);
    QByteArray & insert(int i, const QByteArray & ba);

    QByteArray & remove(int pos, int len);

    QByteArray & replace(int index, char ch);
    QByteArray & replace(int index, const QByteArray & ba);
    QByteArray & replace(int index, int length, const QByteArray & ba);
*/
    QChar AsciiChar(int index);
    //QString ToReadableString(int start=0, int end=-1);
    int bytesperline;

signals:

public slots:

private:
    QByteArray _data;
    //QByteArray _changedData;

    int addressnumbers;                    // wanted width of address area
    int addressoffset;                     // will be added to the real addres inside bytearray
    int realaddressnumbers;                // real width of address area (can be greater then wanted width)
    //int _oldSize;                           // size of data
    TskObject* tskptr;
    off_t imagesize; // bytes per image
    off_t blocksize; // bytes per image block
    off_t currentoffset; // current cursor offset
    off_t firstoffset; // first offset for image
    off_t lastoffset; // last offset for image
    off_t slicestart; // first loaded offset
    off_t sliceend; // last loaded offset
    off_t slicesize; // size of loaded portion of the image
    off_t sliceindex; // which slice is currently loaded.
    off_t linecount;
    off_t blocklinecount;
};

/** \endcond docNever */
#endif // XBYTEARRAY_H
