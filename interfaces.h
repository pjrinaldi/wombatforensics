/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtPlugin>

/*
class QImage;
class QPainter;
class QWidget;
class QPainterPath;
class QPoint;
class QRect;
class QString;
class QStringList;
*/

/*
 *
 *
 * CLEARLY NEED TO CUSTOMIZE FOR OUR PURPOSES...
 * THE TYPES SHOULD PROBABLY BE EXTRACTION, ANALYSIS, PRE-PROCESS, POST-PROCESS, ETC
 * (RATHER THAN BRUSH, SHAPE, FILTER)
 *
 *
 *
 */
class EvidenceInterface
{
public:
    virtual ~EvidenceInterface() {}

    virtual QStringList evidenceMenuActions() const = 0;
    virtual QStringList evidenceToolbarActions() const = 0;
    virtual QStringList toolboxViews() const = 0;
    virtual QStringList evidenceToolbarIcons() const = 0;
    virtual QStringList evidenceToolboxIcons() const = 0;

    virtual QWidget* setupToolBox() const = 0;
    virtual QWidget* setupTabWidget() const = 0;
    virtual QWidget* setupColumnView() const = 0;

    virtual void addEvidence() = 0;
    virtual void remEvidence() = 0;

};

#endif

#define EvidenceInterface_iid "wombat.forensics.EvidenceInterface"

Q_DECLARE_INTERFACE(EvidenceInterface, EvidenceInterface_iid)

/*
class BrushInterface
{
public:
    virtual ~BrushInterface() {}

    virtual QStringList brushes() const = 0;
    virtual QRect mousePress(const QString &brush, QPainter &painter,
                             const QPoint &pos) = 0;
    virtual QRect mouseMove(const QString &brush, QPainter &painter,
                            const QPoint &oldPos, const QPoint &newPos) = 0;
    virtual QRect mouseRelease(const QString &brush, QPainter &painter,
                               const QPoint &pos) = 0;
};

class ShapeInterface
{
public:
    virtual ~ShapeInterface() {}

    virtual QStringList shapes() const = 0;
    virtual QPainterPath generateShape(const QString &shape,
                                       QWidget *parent) = 0;
};

class FilterInterface
{
public:
    virtual ~FilterInterface() {}

    virtual QStringList filters() const = 0;
    virtual QImage filterImage(const QString &filter, const QImage &image,
                               QWidget *parent) = 0;
};

#define BrushInterface_iid "org.qt-project.Qt.Examples.PlugAndPaint.BrushInterface"

Q_DECLARE_INTERFACE(BrushInterface, BrushInterface_iid)

#define ShapeInterface_iid  "org.qt-project.Qt.Examples.PlugAndPaint.ShapeInterface"

Q_DECLARE_INTERFACE(ShapeInterface, ShapeInterface_iid)
#define FilterInterface_iid "org.qt-project.Qt.Examples.PlugAndPaint.FilterInterface"

Q_DECLARE_INTERFACE(FilterInterface, FilterInterface_iid)
*/
