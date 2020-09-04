#include "imagefunctions.h"

unsigned long long GetTotalBytes(std::string instr)
{
    unsigned long long totbyt = 0;
    //printf("INFILE: %s\n", instr.c_str());
    int infile = open(instr.c_str(), O_RDONLY);
    ioctl(infile, BLKGETSIZE64, &totbyt);
    close(infile);
    //printf("total bytes: %lld\n", totbyt);
    return totbyt;
}

void ReadBytes(std::string instr, std::string outstr)
{
    unsigned long long totalbytes = 0;
    unsigned long long curpos = 0;
    int infile = open(instr.c_str(), O_RDONLY);
    int outfile = open(outstr.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRWXU);
    ioctl(infile, BLKGETSIZE64, &totalbytes);
    lseek(infile, 0, SEEK_SET);
    lseek(outfile, 0, SEEK_SET);
    while (curpos < totalbytes)
    {
        char bytebuf[512];
	ssize_t bytesread = read(infile, bytebuf, 512);
	//printf("bytes read: %ld\n", bytesread);
	//printf("Bytes: %x %x %x\n", bytebuf[0], bytebuf[1], bytebuf[2]);
	ssize_t byteswrite = write(outfile, bytebuf, 512);
	//printf("bytes written: %ld\n", byteswrite);
	if(byteswrite == -1)
	    perror("Write Error:");
	//printf("error: %s\n", strerror(errno));
	curpos = curpos + 512;
	printf("Written Bytes: %lld out of total bytes: %lld", curpos, totalbytes);
	//printf("Percent complete: %d\n", (int)(((float)curpos / (float)totalbytes) * 100.0));
    }
    printf("\nForensic Image Creation Finished!\n");
    //printf("Bytes: %c %c %c\n", bytebuf[0], bytebuf[1], bytebuf[2]);
    close(infile);
    close(outfile);
}

void Verify(std::string instr, std::string outstr)
{
    printf("\nStarting Image Verification...\n");
    unsigned char c[MD5_DIGEST_LENGTH];
    unsigned char o[MD5_DIGEST_LENGTH];
    int i;
    FILE* infile = fopen(instr.c_str(), "rb");
    FILE* outfile = fopen(outstr.c_str(), "rb");
    MD5_CTX mdcontext;
    MD5_CTX outcontext;
    int bytes;
    int obytes;
    unsigned char data[1024];
    unsigned char odata[1024];
    MD5_Init(&mdcontext);
    MD5_Init(&outcontext);
    while((bytes = fread(data, 1, 1024, infile)) != 0)
	MD5_Update(&mdcontext, data, bytes);
    MD5_Final(c, &mdcontext);
    while((obytes = fread(odata, 1, 1024, outfile)) != 0)
	MD5_Update(&outcontext, data, bytes);
    MD5_Final(o, &outcontext);
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
	printf("%02x", c[i]);
    printf(" %s\n", instr.c_str());
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
	printf("%02x", o[i]);
    printf(" %s\n", outstr.c_str());
    std::string inmd5(c);
    std::string outmd5(o);
    fclose(infile);
    fclose(outfile);
}
