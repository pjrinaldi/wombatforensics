#include "binviewwidget.h"

#include <QPainter>
#include <QScrollBar>

#include "binviewmodel.h"

void BinViewWidget::calculateRowCount()
{
    m_bytesCount = 0 == m_model ? 0 : m_model->size();
    m_rowCount = m_bytesCount / m_bytesPerLine;
    if ((m_bytesCount % m_bytesPerLine) != 0)
    {
        m_rowCount++;
    }
}

void BinViewWidget::prepareCharImage(QPainter* widgetPainter)
{
    if (m_charImage != 0)
    {
        return;
    }

    int textFlags = Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextSingleLine;

    QRect charRect = widgetPainter->boundingRect(rect(), textFlags, QString(QChar(48)));
    m_charWidth = charRect.width();
    m_rowHeight = charRect.height();

    m_charImage = new QImage(m_charWidth * 256, m_rowHeight, QImage::Format_RGB32);

    QPainter charPainter(m_charImage);

    charPainter.setPen(Qt::white);
    charPainter.setBrush(QBrush(Qt::white));
    charPainter.drawRect(m_charImage->rect());

    charPainter.setPen(Qt::black);
    charPainter.setFont(m_font);

    for (int c = 0; c < 256; c++)
    {
        int x0 = c * m_charWidth;

        charPainter.drawText(x0, 0, m_charWidth, m_rowHeight, textFlags, QString(QChar(c)));

//        // draw dots instead of empty spaces
//        if (c != 32)
//        {
//            bool allEmpty = true;

//            QRgb white = QColor(Qt::white).rgb();

//            for (int x = 0; x < m_charWidth; x++)
//            {
//                for (int y = 0; y < m_rowHeight; y++)
//                {
//                    if (m_charImage->pixel(x0 + x, y) != white)
//                    {
//                        allEmpty = false;
//                        break;
//                    }
//                }
//                if (!allEmpty)
//                {
//                    break;
//                }
//            }

//            if (allEmpty)
//            {
//                charPainter.drawText(x0, 0, m_charWidth, m_rowHeight, textFlags, QString(QChar(46)));
//            }
//        }
    }
}

void BinViewWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(viewport());

    painter.setPen(Qt::white);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(rect());

    if (0 == m_model)
    {
        return;
    }

    painter.setPen(Qt::black);
    painter.setFont(m_font);

    prepareCharImage(&painter);

//    painter.drawImage(0, 0, *m_charImage);
//    return;

    if (0 == m_rowHeight)
    {
        return;
    }

    int rows = rect().height() / m_rowHeight;

    verticalScrollBar()->setRange(0, qMax<qint64>(0, m_rowCount - rows));

    qint64 firstRow = verticalScrollBar()->sliderPosition();
    qint64 maxRow = qMin<qint64>(m_rowCount, firstRow + rows);

    int y = rect().top();

    int digitLength = 16 == m_radix ? 2 : 3;

//    int offsetPos = m_charWidth;
//    int bytesPos = offsetPos + (0 == m_offsetMode ? 0 : m_charWidth * 13);
//    int textPos = bytesPos + m_bytesPerLine * digitLength +
//                  (0 == (m_bytesPerLine % 10) ? (m_bytesPerLine * 6 / 5) : (m_bytesPerLine * 5 / 4)) + 1;

    QChar fillerChar(10 == m_radix ? ' ' : '0');

    for (qint64 row = firstRow; row < maxRow; row++)
    {
        qint64 firstByte = row * m_bytesPerLine;

        QByteArray bytes = m_model->read(firstByte, m_bytesPerLine);
        int bytesLength = bytes.length();

        QString line = " ";

        if (m_offsetMode != 0)
        {
            line += changeHexCase(QString("%1  ").arg(firstByte, 10, m_offsetMode, QLatin1Char('0')), 16 == m_offsetMode);
        }

        for (int i = 0; i < m_bytesPerLine; i++)
        {
            if (0 == (i % ((0 == m_bytesPerLine % 10    ) ? 5 : 4)))
            {
                line += ' ';
            }

            if (i < bytesLength)
            {
                quint8 c = (quint8)bytes.at(i);
                line += changeHexCase(QString(" %1").arg(c, digitLength, m_radix, fillerChar), 16 == m_radix);
            }
            else
            {
                line += "   ";
            }
        }

        line += "    ";

        for (int i = 0; i < line.length(); i++)
        {
            painter.drawImage(i * m_charWidth, y, *m_charImage, line.at(i).unicode() * m_charWidth, 0, m_charWidth, m_rowHeight);
        }

        int x0 = line.length() * m_charWidth;
        for (int i = 0; i < bytesLength; i++)
        {
            quint8 c = (quint8)bytes.at(i);
            painter.drawImage(x0 + i * m_charWidth, y, *m_charImage, c * m_charWidth, 0, m_charWidth, m_rowHeight);
        }

        y += m_rowHeight;
    }
}

void BinViewWidget::scrollContentsBy(int /*dx*/, int /*dy*/)
{
    viewport()->update();
}

BinViewWidget::BinViewWidget(QWidget* parent) : QAbstractScrollArea(parent)
{
    m_model = 0;
    m_rowCount = 0;
    m_bytesCount = 0;

    m_bytesPerLine = 16;
    m_offsetMode = 16;

    m_radix = 16;
    m_upperHex = true;

    m_font = QFont("Courier", 12);

    m_charImage = 0;
    m_rowHeight = 0;
    m_charWidth = 0;
}

QString BinViewWidget::changeHexCase(const QString& text, bool isHex)
{
    if (isHex)
    {
        return m_upperHex ? text.toUpper() : text.toLower();
    }
    else
    {
        return text;
    }
}

// data

void BinViewWidget::setModel(BinViewModel* model)
{
    m_model = model;

    calculateRowCount();

    viewport()->update();
}

qint64 BinViewWidget::rowCount() const
{
    return m_rowCount;
}

qint64 BinViewWidget::bytesCount() const
{
    return m_bytesCount;
}

// bytesPerLine

int BinViewWidget::bytesPerLine()
{
    return m_bytesPerLine;
}

void BinViewWidget::setBytesPerLine(int bytesPerLine)
{
    m_bytesPerLine = bytesPerLine;

    if (0 == m_model)
    {
        return;
    }

    calculateRowCount();

    viewport()->update();
}

// offset

int BinViewWidget::offsetMode()
{
    return m_offsetMode;
}

void BinViewWidget::setOffsetMode(int offsetMode)
{
    m_offsetMode = offsetMode;

    if (0 == m_model)
    {
        return;
    }

    viewport()->update();
}

// radix

int BinViewWidget::radix()
{
    return m_radix;
}

void BinViewWidget::setRadix(int radix)
{
    m_radix = radix;

    if (0 == m_model)
    {
        return;
    }

    viewport()->update();
}

// font

QFont BinViewWidget::font()
{
    return m_font;
}

void BinViewWidget::setFont(QFont font)
{
    m_font = font;

    if (0 == m_model)
    {
        return;
    }

    delete m_charImage;
    m_charImage = 0;

    viewport()->update();
}

// upperHex

bool BinViewWidget::upperHex()
{
    return m_upperHex;
}

void BinViewWidget::setUpperHex(int upperHex)
{
    m_upperHex = upperHex;

    if (0 == m_model)
    {
        return;
    }

    viewport()->update();
}

