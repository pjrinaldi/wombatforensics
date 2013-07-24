#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include <interfaces.h>
#include <QStringList>
#include <QtWidgets>

class BasicTools : public QObject, public EvidenceInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "wombat.forensics.EvidenceInterface" FILE "BasicTools.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(EvidenceInterface)
    
public:
    //Evidence Interface
    QStringList evidenceActions() const;
    QStringList toolboxViews() const;
    QWidget* setupToolBox() const;

    void addEvidence();
    void remEvidence();

//    BasicTools(QObject *parent = 0);
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
