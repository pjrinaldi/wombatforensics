#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include ""

class BasicTools :
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QSqlDriverFactoryInterface" FILE "BasicTools.json")
#endif // QT_VERSION >= 0x050000
    
public:
    BasicTools(QObject *parent = 0);
};

#endif // BASICTOOLS_H
