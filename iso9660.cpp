#include "iso9660.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

void ParseIsoDirectory(ForImg* curimg, uint32_t curstartsector, uint8_t ptreecnt)
{
    // PROCESS IS TO PARSE SMALL PATH TABLE RECORD, WHICH LEADS TO THE DIRECTORY RECORD WHICH THEN TAKES ME TO THE FILE RECORD INFO???
    qDebug() << "parse iso:" << curstartsector << ptreecnt;
}
