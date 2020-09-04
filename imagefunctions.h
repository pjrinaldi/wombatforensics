#include <fcntl.h>
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <errno.h>
#include <stdio.h>
#include <openssl/md5.h>

unsigned long long GetTotalBytes(std::string infile);
void ReadBytes(std::string infile, std::string outfile);
void Verify(std::string instr, std::string outstr);
