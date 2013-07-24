#include "basictools.h"


BasicTools::BasicTools(QObject *parent) :
    QSqlDriverPlugin(parent)
{
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(BasicTools, BasicTools)
#endif // QT_VERSION < 0x050000
