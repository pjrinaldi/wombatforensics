#ifndef SLEUTHFILEITEM_H
#define SLEUTHFILEITEM_H

#include <QStandardItem>

class SleuthFileItem : public QStandardItem
{
public:
    SleuthFileItem();
    int GetSleuthFileID();
    void SetSleuthFileID(int fileid);

private:
    sleuthfileid;
};

#endif // EVIDENCE_H
