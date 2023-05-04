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
#include <lexbor/html/tokenizer.h> // HTML PARSING

static lxb_html_token_t* token_callback(lxb_html_tokenizer_t *tkz, lxb_html_token_t *token, void *ctx);

void ParsePreview(ForImg* curforimg, CurrentItem* curitem, FileItem* curfileitem, uint8_t* prebuf, uint64_t bufsize, std::string* filecontents);
void ParseArtifact(ForImg* curforimg, CurrentItem* curitem, FileItem* curfileitem, bool* inmemory, uint8_t* tmpbuf, FILE* tmpfile, std::string* filecontents);
void GetXmlText(rapidxml::xml_node<>* curnode, std::string* contents);

void ParsePdf(FileItem* curfileitem, std::string* filecontents);
void ParseHtml(uint8_t* prebuf, uint64_t bufsize, std::string* filecontents);
void ParseRecycler(FileItem* curfileitem, uint8_t* prebuf, uint64_t bufsize, std::string* filecontents);
void ParseRecycleBin(FileItem* curfileitem, uint8_t* prebuf, std::string* filecontents);
void ParseDirectory(CurrentItem* curitem, FileItem* curfileitem, std::string* filecontents);
void ParseDocx(FileItem* curfileitem, std::string* filecontents);
void ParsePrefetch(FileItem* curfileitem, uint8_t* tmpbuf, std::string* filecontents);
void ParseShortcut(FileItem* curfileitem, uint8_t* tmpbuf, std::string* filecontents);
void ParseImage(FileItem* curfileitem, std::string* filecontents);

#endif // ARTIFACTPARSER_H
