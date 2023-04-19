#ifndef ARTIFACTPARSER_H
#define ARTIFACTPARSER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include "common.h"
#include "directories.h"
#include "forensicimage.h"

#include "rapidxml.hpp" // XML PARSING
#include <zip.h> // ZIP PARSING
#include <libfwsi.h> // SHELL ITEM PARSING for LNK
#include <libfwnt.h> // LZEXPRESS HUFFMAN for PF
#include "/usr/include/poppler/cpp/poppler-document.h" // PDF PARSING
#include "/usr/include/poppler/cpp/poppler-page.h" // PDF PARSING
#include <hubbub/parser.h> // HTML PARSING
//#include <hubbub/tree.h> // HTML PARSING
#include <litehtml/litehtml.h> // HTML PARSING

void ParseArtifact(ForImg* curforimg, CurrentItem* curitem, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents);
void GetXmlText(rapidxml::xml_node<>* curnode, std::string* contents);

#endif // ARTIFACTPARSER_H
