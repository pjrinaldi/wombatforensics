#ifndef BINVIEWWIDGET_H
#define BINVIEWWIDGET_H

#include <QAbstractScrollArea>

#include "binviewmodel.h"

class BinViewWidget : public QAbstractScrollArea
{
    BinViewModel* m_model;
    qint64 m_rowCount;
    qint64 m_bytesCount;

    int m_bytesPerLine;
    int m_offsetMode;

    int m_radix;
    bool m_upperHex;

    QFont m_font;

    void calculateRowCount();

    QImage* m_charImage;
    int m_rowHeight;
    int m_charWidth;
    void prepareCharImage(QPainter* widgetPainter);

protected:

    virtual void paintEvent(QPaintEvent* /*event*/);

    void scrollContentsBy(int /*dx*/, int /*dy*/);

public:

    BinViewWidget(QWidget* parent = 0);

    QString changeHexCase(const QString& text, bool isHex = true);

    // data
    void setModel(BinViewModel* model);
    qint64 rowCount() const;
    qint64 bytesCount() const;

    // bytesPerLine
    int bytesPerLine();
    void setBytesPerLine(int bytesPerLine);

    // offset
    int offsetMode();
    void setOffsetMode(int offsetMode);

    // radix
    int radix();
    void setRadix(int radix);

    // font
    QFont font();
    void setFont(QFont font);

    // upperHex
    bool upperHex();
    void setUpperHex(int upperHex);
};

#endif // BINVIEWWIDGET_H
