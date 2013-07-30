#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include <interfaces.h>
#include "wombattskimagefiletsk.h"
#include "tskfunctions.h"
#include "tsk/framework/extraction/TskImageFileTsk.h"
#include "tsk/framework/services/TskImgDBSqlite.h"
#include "tsk/framework/services/TskSchedulerQueue.h"
#include "tsk/framework/file/TskFileManagerImpl.h"
#include <QStringList>
#include <QtWidgets>
#include <QFileDialog>

class BasicTools : public QObject, public EvidenceInterface, public BasicToolsInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "wombat.forensics.EvidenceInterface" FILE "BasicTools.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(EvidenceInterface BasicToolsInterface)
    
public:
    //Evidence Interface
    QStringList evidenceActions() const;
    QStringList evidenceActionIcons() const;

    void addEvidence(int currentCaseID);
    void remEvidence(int currentCaseID);
    QStringList toolboxViews() const;
    QStringList evidenceToolboxIcons() const;

    QWidget* setupToolBox() const;
    QWidget* setupTabWidget() const;
    QWidget* setupColumnView() const;

private:
    sqlite3_stmt *sqlStatement;
    int currentcaseid;

};

#endif // BASICTOOLS_H

/*
#ifndef BASICTOOLSPLUGIN_H
#define BASICTOOLSPLUGIN_H

#include <QRect>
#include <QObject>
#include <QtPlugin>
#include <QStringList>
#include <QPainterPath>
#include <QImage>

#include <plugandpaint/interfaces.h>

class BasicToolsPlugin : public QObject,
                         public BrushInterface,
                         public ShapeInterface,
                         public FilterInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface" FILE "basictools.json")
    Q_INTERFACES(BrushInterface ShapeInterface FilterInterface)

public:
    // BrushInterface
    QStringList brushes() const;
    QRect mousePress(const QString &brush, QPainter &painter,
                     const QPoint &pos);
    QRect mouseMove(const QString &brush, QPainter &painter,
                    const QPoint &oldPos, const QPoint &newPos);
    QRect mouseRelease(const QString &brush, QPainter &painter,
                       const QPoint &pos);

    // ShapeInterface
    QStringList shapes() const;
    QPainterPath generateShape(const QString &shape, QWidget *parent);

    // FilterInterface
    QStringList filters() const;
    QImage filterImage(const QString &filter, const QImage &image,
                       QWidget *parent);
};

#endif
 */
