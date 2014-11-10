#include "hexviewer.h"

const int HEXCHARS_IN_LINE = 47;
const int GAP_ADR_HEX = 10;
const int GAP_HEX_ASCII = 16;
const int BYTES_PER_LINE = 16;

HexViewer::HexViewer(QWidget* parent) : QScrollArea(parent)
{
}

XByteArray& HexViewer::XData(void)
{
    return xdata;
}
void HexViewer::keyPressEvent(QKeyEvent* e)
{
    int charx = (cursorx - hexposition)/charwidth;
    int posx = (charx/3)*2 + (charx % 3);
    int posba = (cursory/charheight) * BYTES_PER_LINE + posx/2;

    // CURSOR MOVEMENTS
    if(e->matches(QKeySequence::MoveToNextChar))
    {
        SetCursorPosition(cursorposition+1);
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToPreviousChar))
    {
        SetCursorPosition(cursorposition-1);
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToEndOfLine))
    {
        SetCursorPosition(cursorposition | (2*BYTES_PER_LINE - 1));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToStartOfLine))
    {
        SetCursorPosition(cursorposition - (cursorposition % (2*BYTES_PER_LINE)));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToPreviousLine))
    {
        SetCursorPosition(cursorposition - (2 * BYTES_PER_LINE));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToNextLine))
    {
        SetCursorPosition(cursorposition + (2 * BYTES_PER_LINE));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToNextPage))
    {
        SetCursorPosition(cursorposition + (((scrollarea->viewport()->height() / charheight) - 1) * 2 * BYTES_PER_LINE));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToPreviousPage))
    {
        SetCursorPosition(cursorposition - (((scrollarea->viewport()->height() / charheight) - 1) * 2 * BYTES_PER_LINE));
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToEndOfDocument))
    {
        SetCursorPosition(bytedata.size() * 2);
        ResetSelection(cursorposition);
    }
    if(e->matches(QKeySequence::MoveToStartOfDocument))
    {
        SetCursorPosition(0);
        ResetSelection(cursorposition);
    }
    // SELECT COMMANDS
    if (e->matches(QKeySequence::SelectAll))
    {
        ResetSelection(0);
        SetSelection(2*bytedata.size() + 1);
    }
    if (e->matches(QKeySequence::SelectNextChar))
    {
        int pos = cursorposition + 1;
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectPreviousChar))
    {
        int pos = cursorposition - 1;
        SetSelection(pos);
        SetCursorPosition(pos);
    }
    if (e->matches(QKeySequence::SelectEndOfLine))
    {
        int pos = cursorposition - (cursorposition % (2 * BYTES_PER_LINE)) + (2 * BYTES_PER_LINE);
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectStartOfLine))
    {
        int pos = cursorposition - (cursorposition % (2 * BYTES_PER_LINE));
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectPreviousLine))
    {
        int pos = cursorposition - (2 * BYTES_PER_LINE);
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectNextLine))
    {
        int pos = cursorposition + (2 * BYTES_PER_LINE);
        SetCursorPosition(pos);
        SetSelection(pos);
    }

    if (e->matches(QKeySequence::SelectNextPage))
    {
        int pos = cursorposition + (((scrollarea->viewport()->height() / charheight) - 1) * 2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
    }
    if (e->matches(QKeySequence::SelectPreviousPage))
    {
        int pos = cursorposition - (((scrollarea->viewport()->height() / charheight) - 1) * 2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
    }
    if (e->matches(QKeySequence::SelectEndOfDocument))
    {
        int pos = bytedata.size() * 2;
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectStartOfDocument))
    {
        int pos = 0;
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    EnsureVisible();
    update();
}

void HexViewer::mouseMoveEvent(QMouseEvent* e)
{
    update();
    int actpos = CursorPosition(e->pos());
    SetCursorPosition(actpos);
    SetSelection(actpos);
}

void HexViewer::mousePressEvent(QMouseEvent* e)
{
    update();
    int cpos = CursorPosition(e->pos());
    ResetSelection(cpos);
    SetCursorPosition(cpos);
}

void HexViewer::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    painter.fillRect(e->rect(), this->palette().color(QPalette::Base));
    painter.fillRect(QRect(labelposition, e->rect().top(), hexposition - GAP_ADR_HEX + 2, height()), addressareacolor);
    int lineposition = asciiposition - (GAP_HEX_ASCII/2);
    painter.setPen(Qt::gray);
    painter.drawLine(lineposition, e->rect().top(), lineposition, height());

    painter.setPen(this->palette.color(QPalette::WindowText));

    int firstlineindex = ((e->rect().top()/charheight) - charheight) * BYTES_PER_LINE;
    if(firstlineindex < 0)
        firstlineindex = 0;
    int lastlineindex = ((e->rect().botto()/charheight) + charheight) * BYTES_PER_LINE;
    if(lastlineindex > bytedata.size())
        lastlineindex = bytedata.size();
    int yposstart = ((firstlineindex)/BYTES_PER_LINE) * charheight + charheight;

    // PAINT ADDRESS AREA
    for(int lineindex = firstlineindex, ypos = yposstart; lineindex < lastlineindex; lindeindex += BYTES_PER_LINE, ypos += charheight)
    {
        QString address = QString("%1").arg(lineindex + xdata.addressOffset(), xdata.realAddressNumbers(), 16, QChar('0'));
        painter.drawText(xposaddr, ypos, address);
    }

    // PAINT HEX AREA

    // PAINT ASCII AREA

    // PAINT CURSOR
}
