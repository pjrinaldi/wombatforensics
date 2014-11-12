#include "xbytearray.h"

XByteArray::XByteArray()
{
    //_oldSize = -99;
    addressnumbers = 4;
    addressoffset = 0;
    slicesize = 819200; // default loaded size is three slices (1600*512)
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
    // initialize the variables
    tskptr = tskpointer;
    blocksize = tskptr->blocksize;
    imagesize = tskptr->imglength;
    sliceindex = 1;
    firstoffset = 0;
    slicestart = slicesize;
    // determine if slicesize is > imagesize and adjust accordingly
    if(slicesize*3 >= imagesize)
    {
        slicesize = imagesize;
        sliceend = slicesize - 1;
    }
    else
    {
        sliceend = 3*slicesize - 1;
    }
    lastoffset = imagesize - 1;
    currentoffset = 0;
    blocklinecount = blocksize / bytesperline;
    linecount = imagesize / bytesperline;
    // load the 1st three slices here...
    int retval = 0;
    LoadSlice(firstoffset, 0);
    LoadSlice(slicesize, 1);
    LoadSlice(2*slicesize, 2);
}

bool XByteArray::LoadSlice(off_t soffset, off_t sindex)
{
    off_t retval = 0;
    char tmpbuf[slicesize];
    retval = tsk_img_read(tskptr->readimginfo, sindex*slicesize, tmpbuf, slicesize);
    qDebug() << "retval" << retval;
    if(retval > 0)
    {
        //slicelist.append(QByteArray(tmpbuf, retval));
        _data.append(QByteArray(tmpbuf, retval));
        qDebug() << "_data size" << _data.size();
    }
    return true;
    /*
    //retval = tsk_img_read(tskptr->readimginfo, sliceoffset + sliceindex*slicesize, NULL, slicesize);
    if(retval > 0)
    {
        if(pageindex < firstpage)
            firstpage = pageindex;
        if(pageindex > lastpage)
            lastpage = pageindex;
    }
    */
}

void XByteArray::FreeSlice(off_t sliceindex)
{
    /*
    if(currentoffset == slicestart)
    {
        // free the 
    }
    */
    // NEED TO FIGURE OUT HOW TO TELL WHERE THE CURRENT OFFSET IS AT
    // IF ITS == SLICE START THEN LOAD SLICEINDEX - 1
    // IF ITS == SLICE END THEN LOAD SLICEINDEX + 1
}
