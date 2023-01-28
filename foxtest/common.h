#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <bitset>

#include "forensicimage.h"

#define DTTMFMT "%F %T %z"
#define DTTMSZ 35

//std::string ConvertUnixTimeToHuman(uint32_t unixtime);

char* GetDateTime(char *buff);
uint8_t* substr(uint8_t* arr, int begin, int len);

void ReadContent(std::ifstream* rawcontent, uint8_t* tmpbuf, uint64_t offset, uint64_t size);

//void ReadContent(std::ifstream* rawcontent, int8_t* tmpbuf, uint64_t offset, uint64_t size);
void ReturnUint32(uint32_t* tmp32, uint8_t* tmp8);
void ReturnUint16(uint16_t* tmp16, uint8_t* tmp8);
void ReturnUint64(uint64_t* tmp64, uint8_t* tmp8);

void ReadContent(std::ifstream* rawcontent, uint16_t* val, uint64_t offset);
void ReadContent(std::ifstream* rawcontent, uint32_t* val, uint64_t offset);
void ReadContent(std::ifstream* rawcontent, uint64_t* val, uint64_t offset);

void ReadForImgContent(ForImg* forimg, uint16_t* val, uint64_t off);
void ReadForImgContent(ForImg* forimg, uint32_t* val, uint64_t off);
void ReadForImgContent(ForImg* forimg, uint64_t* val, uint64_t off);

void ReadInteger(uint8_t* arr, int begin, uint16_t* val);
void ReadInteger(uint8_t* arr, int begin, uint32_t* val);
void ReadInteger(uint8_t* arr, int begin, uint64_t* val);

//void ReturnUint(uint64_t* tmp, uint8_t* tmp8, unsigned int length);
//void ReturnInt(int64_t* tmp, int8_t* tmp8, unsigned int length);

#endif
