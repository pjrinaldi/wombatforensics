#ifndef SLEUTHFILEITEM_H
#define SLEUTHFILEITEM_H

#include <QStandardItem>

class SleuthFileItem : public QStandardItem
{

public:
    SleuthFileItem(QString itemName, int fileID);
    int GetSleuthFileID();
    void SetSleuthFileID(int fileID);

private:
    int sleuthfileid;
};

#endif // EVIDENCE_H
