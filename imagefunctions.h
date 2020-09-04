#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string>

unsigned long long GetTotalBytes(std::string infile);
void ReadBytes(std::string infile, std::string outfile);
