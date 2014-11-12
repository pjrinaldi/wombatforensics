#include "xbytearray.h"

XByteArray::XByteArray()
{
    //_oldSize = -99;
    addressnumbers = 4;
    addressoffset = 0;
    loadedsize = 81920; // default loaded size 1600*512 (linecount * bytes)

}

int XByteArray::AddressOffset()
{
    return addressoffset;
}

void XByteArray::SetAddressOffset(int offset)
{
    addressoffset = offset;
}

int XByteArray::AddressWidth()
{
    return addressnumbers;
}

void XByteArray::SetAddressWidth(int width)
{
    if ((width >= 0) and (width<=6))
    {
        addressnumbers = width;
    }
}

QByteArray & XByteArray::data()
{
    return _data;
}

void XByteArray::SetData(QByteArray d)
{
    _data = d;
    //_changedData = QByteArray(data.length(), char(0));
}
/*
bool XByteArray::dataChanged(int i)
{
    return bool(_changedData[i]);
}

QByteArray XByteArray::dataChanged(int i, int len)
{
    return _changedData.mid(i, len);
}

void XByteArray::setDataChanged(int i, bool state)
{
    _changedData[i] = char(state);
}

void XByteArray::setDataChanged(int i, const QByteArray & state)
{
    int length = state.length();
    int len;
    if ((i + length) > _changedData.length())
        len = _changedData.length() - i;
    else
        len = length;
    _changedData.replace(i, len, state);
}
*/
int XByteArray::RealAddressNumbers()
{
    // is addressNumbers wide enought?
    QString test = QString("%1").arg(_data.size() + addressoffset, addressnumbers, 16, QChar('0'));
    realaddressnumbers = test.size();
    return realaddressnumbers;
}

int XByteArray::size()
{
    return _data.size();
}
/*
QByteArray & XByteArray::insert(int i, char ch)
{
    _data.insert(i, ch);
    _changedData.insert(i, char(1));
    return _data;
}

QByteArray & XByteArray::insert(int i, const QByteArray & ba)
{
    _data.insert(i, ba);
    _changedData.insert(i, QByteArray(ba.length(), char(1)));
    return _data;
}

QByteArray & XByteArray::remove(int i, int len)
{
    _data.remove(i, len);
    _changedData.remove(i, len);
    return _data;
}

QByteArray & XByteArray::replace(int index, char ch)
{
    _data[index] = ch;
    _changedData[index] = char(1);
    return _data;
}

QByteArray & XByteArray::replace(int index, const QByteArray & ba)
{
    int len = ba.length();
    return replace(index, len, ba);
}

QByteArray & XByteArray::replace(int index, int length, const QByteArray & ba)
{
    int len;
    if ((index + length) > _data.length())
        len = _data.length() - index;
    else
        len = length;
    _data.replace(index, len, ba.mid(0, len));
    _changedData.replace(index, len, QByteArray(len, char(1)));
    return _data;
}
*/
QChar XByteArray::AsciiChar(int index)
{
    char ch = _data[index];
    if ((ch < 0x20) or (ch > 0x7e))
            ch = '.';
    return QChar(ch);
}
/*
QString XByteArray::ToReadableString(int start, int end)
{
    int adrWidth = RealAddressNumbers();
    if (addressnumbers > adrWidth)
        adrWidth = addressnumbers;
    if (end < 0)
        end = _data.size();

    QString result;
    for (int i=start; i < end; i += 16)
    {
        QString adrStr = QString("%1").arg(addressoffset + i, adrWidth, 16, QChar('0'));
        QString hexStr;
        QString ascStr;
        for (int j=0; j<16; j++)
        {
            if ((i + j) < _data.size())
            {
                hexStr.append(" ").append(_data.mid(i+j, 1).toHex());
                ascStr.append(AsciiChar(i+j));
            }
        }
        result += adrStr + " " + QString("%1").arg(hexStr, -48) + "  " + QString("%1").arg(ascStr, -17) + "\n";
    }
    return result;
}
*/

int XByteArray::LineCount(void)
{
    return linecount;
}

int XByteArray::BlockLineCount(void)
{
    return blocklinecount;
}

bool XByteArray::OpenImage(TskObject* tskpointer)
{
    // initialize the line variables and the loadedline variables
    tskptr = tskpointer;
    blocksize = tskptr->blocksize;
    imagesize = tskptr->imglength;
    // determine if loadedsize is > imagesize and adjust accordingly
    if(loadedsize >= imagesize)
        loadedsize = imagesize;
    loadedlinecount = loadedsize / bytesperline;
    linecount = imagesize / bytesperline;
    blocklinecount = blocksize / bytesperline;
    firstline = 0;
    lastline = linecount - 1;
    firstloadedline = 0;
    lastloadedline = loadedlinecount - 1;
    
    //_data.resize(pagecount);
    //_data.fill('0');
    // IN HEXEDITOR/READER PARADIGM, DATA IS A VECTOR OF UCHAR*'S OF 512 BYTES.
    // IN HEXVIEWER/XBYTEARRAY PARADIGM, DATA IS THE FULL BYTEARRAY OF THE IMAGE.
    //firstpage = lastpage = 0;

    return LoadImagePage(0);
}

bool XByteArray::LoadImagePage(off_t pageindex)
{
    off_t retval = 0;
    // freepages...
    //retval = tsk_img_read(tskptr->readimginfo, tskptr->offset + pageindex*pagesize, NULL, pagesize);
    if(tskptr->objecttype > 1)
    {
        // do highlighting here or set it up.
    }
    if(retval > 0)
    {
        /*
        if(pageindex < firstpage)
            firstpage = pageindex;
        if(pageindex > lastpage)
            lastpage = pageindex;
        */
    }
    return true;
}
