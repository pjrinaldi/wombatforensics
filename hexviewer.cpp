#include "hexviewer.h"

const int HEXCHARS_IN_LINE = 47;
const int GAP_ADR_HEX = 10;
const int GAP_HEX_ASCII = 16;
const int BYTES_PER_LINE = 16;

HexViewer::HexViewer(QWidget* parent) : QScrollArea(parent)
{
}

void HexViewer::keyPressEvent(QKeyEvent* e)
{
    int charx = (cursorx - hexposition)/charwidth;
    int posx = (charx/3)*2 + (charx % 3);
    int posba = (cursory/charheight) * BYTES_PER_LINE + posx/2;
}
