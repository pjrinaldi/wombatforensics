#include "filecarvers.h"

void CarveJPEG(QString curcarvetype)
{
    QString curheadcat = curcarvetype.split(",").at(0);
    QString curheadstr = curcarvetype.split(",").at(2);
    QString curheadname = curcarvetype.split(",").at(1);
    QString curfootstr = curcarvetype.split(",").at(3);
    QString curextstr = curcarvetype.split(",").at(4);
    int bytecount = curheadstr.count() / 2;
    int headleft = curheadstr.indexOf("?");
    int headright = curheadstr.lastIndexOf("?");
}
