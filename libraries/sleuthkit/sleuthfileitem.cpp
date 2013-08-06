#include "sleuthfileitem.h"

SleuthFileItem::SleuthFileItem(QString itemName, int fileID) : QStandardItem(itemName)
{
    sleuthfileid = fileID;
}
void SleuthFileItem::SetSleuthFileID(int fileID)
{
    sleuthfileid = fileID;
}

int SleuthFileItem::GetSleuthFileID()
{
    return sleuthfileid;
}
