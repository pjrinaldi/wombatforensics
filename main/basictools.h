#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include "binviewwidget.h"
#include <QObject>
#include <QtWidgets>
#include <QStringList>

class BasicTools : public QObject
{
    Q_OBJECT

public:
    // plugin interface functions
    //BasicTools Custom Functions
    BasicTools() {};
    ~BasicTools() {};

    QWidget* setupHexTab();
    QWidget* setupTxtTab();
    QWidget* setupDirTab();
    QWidget* setupTypTab();
    void LoadHexModel(QString tmpFilePath);
    void LoadTxtContent(QString asciiText);
public slots:
    void LoadFileContents(QString filepath);
private:
    BinViewModel* hexmodel;
    BinViewWidget* hexwidget;
    QTreeView* dirtreeview;
    QTreeView* typtreeview;
    QTextEdit* txtwidget;
};

#endif // BASICTOOLS_H
