#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include "hexEditor.hpp"
#include "translate.hpp"
#include <QObject>
#include <QtWidgets>
#include <QStringList>
#include <QSizePolicy>
#include <QFrame>
#include <QStatusBar>

class BasicTools : public QObject
{
    Q_OBJECT

public:
    // plugin interface functions
    //BasicTools Custom Functions
    BasicTools() {};
    ~BasicTools() {};

    off_t offset() const;

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
    void setOffsetLabel(off_t pos);
    void UpdateSelectValue(const QString &txt);
private:
    HexEditor* hexwidget;
    QScrollBar* hexvsb;
    QStatusBar* hstatus;
    QLabel* selectedoffset;
    QLabel* selectedhex;
    QLabel* selectedascii;
    QLabel* selectedinteger;
    QLabel* selectedfloat;
    QLabel* selecteddouble;
    QTreeView* dirtreeview;
    QTreeView* typtreeview;
    QTextEdit* txtwidget;
};

#endif // BASICTOOLS_H
