#include "hexviewer.h"

const int HEXCHARS_IN_LINE = 47;
const int GAP_ADR_HEX = 10;
const int GAP_HEX_ASCII = 16;
const int BYTES_PER_LINE = 16;

HexViewer::HexViewer(QWidget* parent, TskObject* tskobjptr) : QWidget(parent)
{
    tskptr = tskobjptr;
    SetAddressWidth(4);
    SetAddressOffset(0);
    addressareacolor = QColor(255, 0, 0, 100);
    highlightcolor = QColor(0, 255, 0, 100);
    selectioncolor = QColor(0, 0, 255, 100);
    setFont(QFont("fixed", 10));

    size = 0;
    ResetSelection(0);
}

XByteArray& HexViewer::XData(void)
{
    return xdata;
}

void HexViewer::SetAddressWidth(int addresswidth)
{
    xdata.bytesperline = BYTES_PER_LINE;
    xdata.SetAddressWidth(addresswidth);
    SetCursorPosition(cursorposition);
}

void HexViewer::SetAddressOffset(int addressoffset)
{
    xdata.SetAddressOffset(addressoffset);
    Adjust();
}

void HexViewer::SetData(const QByteArray &data)
{
    xdata.SetData(data);
    Adjust();
    SetCursorPosition(0);
}

QByteArray HexViewer::Data()
{
    return xdata.data();
}

void HexViewer::keyPressEvent(QKeyEvent* e)
{
    //int charx = (cursorx - hexposition)/charwidth;
    //int posx = (charx/3)*2 + (charx % 3);
    //int posba = (cursory/charheight) * BYTES_PER_LINE + posx/2;

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
        SetCursorPosition(xdata.size() * 2);
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
        SetSelection(2*xdata.size() + 1);
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
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectPreviousPage))
    {
        int pos = cursorposition - (((scrollarea->viewport()->height() / charheight) - 1) * 2 * BYTES_PER_LINE);
        SetCursorPosition(pos);
        SetSelection(pos);
    }
    if (e->matches(QKeySequence::SelectEndOfDocument))
    {
        int pos = xdata.size() * 2;
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

    //painter.setPen(this->palette().color(QPalette::WindowText));
    painter.setPen(qApp->palette().foreground().color());

    int firstlineindex = ((e->rect().top()/charheight) - charheight) * BYTES_PER_LINE;
    if(firstlineindex < 0)
        firstlineindex = 0;
    int lastlineindex = ((e->rect().bottom()/charheight) + charheight) * BYTES_PER_LINE;
    if(lastlineindex > xdata.size())
        lastlineindex = xdata.size();
    int yposstart = ((firstlineindex)/BYTES_PER_LINE) * charheight + charheight;

    // PAINT ADDRESS AREA
    for(int lineindex = firstlineindex, ypos = yposstart; lineindex < lastlineindex; lineindex += BYTES_PER_LINE, ypos += charheight)
    {
        QString address = QString("%1").arg(lineindex + xdata.AddressOffset(), xdata.RealAddressNumbers(), 16, QChar('0'));
        painter.drawText(labelposition, ypos, address);
    }

    // PAINT HEX AREA
    QByteArray hexba(xdata.data().mid(firstlineindex, lastlineindex - firstlineindex + 1).toHex());
    QBrush highlighted = QBrush(highlightcolor);
    //QPen colhighlighted = QPen(this->palette().color(QPalette::WindowText));
    QPen colhighlighted = QPen(Qt::black);
    QBrush selected = QBrush(selectioncolor);
    QPen colselected = QPen(Qt::black);
    //QPen colstandard = QPen(this->palette().color(QPalette::WindowText));
    QPen colstandard = QPen(Qt::black);
    painter.setBackgroundMode(Qt::TransparentMode);

    for(int lineindex = firstlineindex, ypos = yposstart; lineindex < lastlineindex; lineindex += BYTES_PER_LINE, ypos += charheight)
    {
        QByteArray hex;
        int xpos = hexposition;
        for(int colindex = 0; ((lineindex + colindex) < xdata.size() and (colindex < BYTES_PER_LINE)); colindex++)
        {
            int posba = lineindex + colindex;
            if((GetSelectionBegin() <= posba) && (GetSelectionEnd() > posba))
            {
                painter.setBackground(selected);
                painter.setBackgroundMode(Qt::OpaqueMode);
                painter.setPen(colselected);
            }
            else
            {
                // if highlighting (which i might use for the highlighting of specific file blocks
                painter.setBackground(highlighted);
                /*
                if(xdata.dataChanged(posba))
                {
                    painter.setPen(colhighlighted);
                    painter.setBackgroundMode(Qt::OpaqueMode);
                }
                else
                {
                    painter.setPen(colstandard);
                    painter.setBackgroundMode(Qt::TransparentMode);
                }
                */
            }
            if(colindex == 0)
            {
                hex = hexba.mid((lineindex - firstlineindex)*2, 2);
                painter.drawText(xpos, ypos, hex);
                xpos += 2 * charwidth;
            }
            else
            {
                hex = hexba.mid((lineindex + colindex - firstlineindex)*2, 2).prepend(" ");
                painter.drawText(xpos, ypos, hex);
                xpos += 3 * charwidth;
            }
        }
    }
    painter.setBackgroundMode(Qt::TransparentMode);
    //painter.setPen(this->palette().color(QPalette::WindowText));
    painter.setPen(qApp->palette().foreground().color());

    // PAINT ASCII AREA
    for(int lineindex = firstlineindex, ypos = yposstart; lineindex < lastlineindex; lineindex += BYTES_PER_LINE, ypos += charheight)
    {
        int asciipos = asciiposition;
        for(int colindex = 0; ((lineindex + colindex) < xdata.size() and (colindex < BYTES_PER_LINE)); colindex++)
        {
            painter.drawText(asciipos, ypos, xdata.AsciiChar(lineindex + colindex));
            asciipos += charwidth;
        }
    }
    // PAINT CURSOR
    painter.fillRect(cursorx, cursory, 2, charheight, this->palette().color(QPalette::WindowText));
}

void HexViewer::SetCursorPosition(int position)
{
    update();
    if(position > xdata.size() * 2)
        position = xdata.size() * 2;
    if(position < 0)
        position = 0;

    cursorposition = position;
    cursory = (position / (2 * BYTES_PER_LINE)) * charheight + 4;
    int x = (position % (2 * BYTES_PER_LINE));
    cursorx = (((x/2) * 3) + (x % 2)) * charwidth + hexposition;

    update();
    emit CurrentAddressChanged(cursorposition/2);
}

int HexViewer::CursorPosition(QPoint pos)
{
    int result = -1;
    if((pos.x() >= hexposition) and (pos.x() < (hexposition + HEXCHARS_IN_LINE * charwidth)))
    {
        int x = (pos.x() - hexposition) / charwidth;
        if((x % 3) == 0)
            x = (x / 3) * 2;
        else
            x = ((x / 3) * 2) + 1;
        int y = ((pos.y() - 3) / charheight) * 2 * BYTES_PER_LINE;
        result = x + y;
    }
    return result;
}

int HexViewer::CursorPosition(void)
{
    return cursorposition;
}

void HexViewer::ResetSelection(void)
{
    selectionstart = selectioninit;
    selectionend = selectioninit;
}

void HexViewer::ResetSelection(int pos)
{
    if(pos < 0)
        pos = 0;
    pos = pos / 2;
    selectioninit = pos;
    selectionstart = pos;
    selectionend = pos;
}

void HexViewer::SetSelection(int pos)
{
    if(pos < 0)
        pos = 0;
    pos = pos / 2;
    if(pos >= selectioninit)
    {
        selectionend = pos;
        selectionstart = selectioninit;
    }
    else
    {
        selectionstart = pos;
        selectionend = selectioninit;
    }
}

int HexViewer::GetSelectionBegin()
{
    return selectionstart;
}

int HexViewer::GetSelectionEnd()
{
    return selectionend;
}

void HexViewer::UpdateCursor()
{
    update(cursorx, cursory, charwidth, charheight);
}

void HexViewer::Adjust(void)
{
    charwidth = fontMetrics().width(QLatin1Char('9'));
    charheight = fontMetrics().height();

    labelposition = 0;
    hexposition = xdata.RealAddressNumbers() * charwidth + GAP_ADR_HEX;
    asciiposition = hexposition + HEXCHARS_IN_LINE * charwidth + GAP_HEX_ASCII;

    setMinimumHeight(((xdata.size()/16 + 1) * charheight) + 5);
    setMinimumWidth(asciiposition + (BYTES_PER_LINE * charwidth));

    update();
}

void HexViewer::EnsureVisible()
{
    scrollarea->ensureVisible(cursorx, cursory + charheight/2, 3, charheight/2 + 2);
}

bool HexViewer::OpenImage()
{
    if(!xdata.OpenImage(tskptr))
        QMessageBox::critical(this, "HexViewer", "Error Opening Image\n", QMessageBox::Ok, 0);
<<<<<<< Updated upstream
    emit SetRange(0, xdata.LineCount());
    emit StepValues(charheight, xdata.LinesPerPage());
=======
>>>>>>> Stashed changes
    /*
     * SET CURSOR RANGE
     * SET SELECTION TO NOTHING
     * SET NEW SCROLLBAR RANGE
     * CALCULATEFONTMETRICS
     * SETTOPLEFT
    //if(!xdata.OpenImage(
    return true;
    */
    return true;
}

HexViewer::~HexViewer()
{
}
