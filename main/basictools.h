#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include "binviewwidget.h"
#include "hexEditor.hpp"
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
    void setScrollBarRange(off_t low, off_t high);
    void setScrollBarValue(off_t pos);
private:
    HexEditor* hexwidget;
    HexEditor* ascwidget;
    QScrollBar* hexvsb;
    QScrollBar* ascvsb;
    //BinViewModel* hexmodel;
    //BinViewWidget* hexwidget;
    QTreeView* dirtreeview;
    QTreeView* typtreeview;
    QTextEdit* txtwidget;
};

#endif // BASICTOOLS_H
