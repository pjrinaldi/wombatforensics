#include <QtWidgets>

#include "hexview.h"


HexView::HexView(QWidget *parent) : QPlainTextEdit(parent)
{
    /*
        addressarea = new AddressArea(this);

        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(UpdateAddressAreaWidth(int)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(UpdateAddressArea(QRect,int)));
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    
        UpdateAddressAreaWidth(5);
    */
    highlightCurrentLine();
    setFont(QFont("fixed"));
}



int HexView::AddressAreaWidth()
{
    int space = 0;
    if(showaddressarea)
    {
    int digits = 5;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
    }
    return space;
}



void HexView::UpdateAddressAreaWidth(int /* newBlockCount */)
{
    if(showaddressarea)
        setViewportMargins(AddressAreaWidth(), 0, 0, 0);
    else
        setViewportMargins(0, 0, 0, 0);
}



void HexView::UpdateAddressArea(const QRect &rect, int dy)
{
    if (dy)
        addressarea->scroll(0, dy);
    else
        addressarea->update(0, rect.y(), addressarea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        UpdateAddressAreaWidth(0);
}



void HexView::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    /*
    if(showaddressarea)
        addressarea->setGeometry(QRect(cr.left(), cr.top(), AddressAreaWidth(), cr.height()));
        */
}



void HexView::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::yellow).lighter(160);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void HexView::AddressAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(addressarea);
    painter.fillRect(event->rect(), Qt::lightGray);


    /*
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, addressarea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
    */
}
