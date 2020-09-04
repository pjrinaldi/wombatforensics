#include "imagefunctions.h"

unsigned long long GetTotalBytes(std::string instr)
{
    unsigned long long totbyt = 0;
    int infile = open(instr.c_str(), O_RDONLY);
    ioctl(infile, BLKGETSIZE64, &totbyt);
    close(infile);
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
	ssize_t byteswrite = write(outfile, bytebuf, 512);
	if(byteswrite == -1)
	    perror("Write Error:");
	curpos = curpos + 512;
	printf("Wrote %lld out of %lld bytes\r", curpos, totalbytes);
	fflush(stdout);
    }
    printf("\nForensic Image Creation Finished!\n");
    close(infile);
    close(outfile);
}

std::string Verify(std::string instr)
{
    std::string inmd5;
    /*
    FILE* infile = fopen(instr.c_str(), "rb");
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    //char mess1[] = "Test Message\n";
    //char mess2[] = "Hello World\n";
    unsigned char md_value[EVP_MAX_MD_SIZE];
    int md_len, i;

    OpenSSL_add_all_digests();

    md = EVP_md5();
    //md = EVP_get_digestbyname(argv[1]); // will be md = EVP_MD5();
    // may switch to input if the user wants to calculate md5, sha1, or sha256


    mdctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(mdctx, md, NULL);
    while((bytes = fread(data, 1, 1024, instr.c_str())) != 0)
    {
        EVP_DigestUpdate(mdctx, data, bytes);
    }
    //EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
    //EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
    EVP_DigestFinal_ex(mdctx, md_value, &md_len);
    EVP_MD_CTX_destroy(mdctx);

    printf("Digest is: ");
    for(i = 0; i < md_len; i++)
        printf("%02x", md_value[i]);
    printf("\n");

    // Call this once before exit.
    EVP_cleanup();
    */
    return inmd5;
}

void Verify(std::string instr, std::string outstr)
{
    // char* imgbuf = new char[0];
    // imgbuf = new char[fsfile->meta->size];
    // delete[] imgbuf;
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
    {
	MD5_Update(&mdcontext, data, bytes);
    }
    MD5_Final(c, &mdcontext);
    while((obytes = fread(odata, 1, 1024, outfile)) != 0)
    {
	//printf("Forensic Image Bytes Read: %d\r", obytes);
	MD5_Update(&outcontext, odata, obytes);
    }
    MD5_Final(o, &outcontext);
    std::stringstream srcstr;
    std::stringstream imgstr;
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
	srcstr << printf("%02x", c[i]);
	//printf("%02x", c[i]);
    }
    printf(" - MD5 Source Device\n");
    for(i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
	imgstr << printf("%02x", o[i]);
	//printf("%02x", o[i]);
    }
    printf(" - MD5 Forensic Image\n");
    std::string srcmd5 = "";
    std::string imgmd5 = "";
    srcmd5 = srcstr.str();
    imgmd5 = imgstr.str();
    //if(strcmp((const char*)c, (const char*)o) == 0)
    if(srcmd5.compare(imgmd5) == 0)
	printf("Verification Successful\n");
    else
	printf("Verification Failed\n");
    fclose(infile);
    fclose(outfile);
}
/*
 *#include <stdio.h>
 #include <openssl/evp.h>

 main(int argc, char *argv[])
 {
 EVP_MD_CTX *mdctx;
 const EVP_MD *md;
 char mess1[] = "Test Message\n";
 char mess2[] = "Hello World\n";
 unsigned char md_value[EVP_MAX_MD_SIZE];
 int md_len, i;

 OpenSSL_add_all_digests();

 if(!argv[1]) {
        printf("Usage: mdtest digestname\n");
        exit(1);
 }

 md = EVP_get_digestbyname(argv[1]); // will be md = EVP_MD5();
 // may switch to input if the user wants to calculate md5, sha1, or sha256

 if(!md) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
 }

 mdctx = EVP_MD_CTX_create();
 EVP_DigestInit_ex(mdctx, md, NULL);
 EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
 EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
 EVP_DigestFinal_ex(mdctx, md_value, &md_len);
 EVP_MD_CTX_destroy(mdctx);

 printf("Digest is: ");
 for(i = 0; i < md_len; i++)
        printf("%02x", md_value[i]);
 printf("\n");

 // Call this once before exit.
 EVP_cleanup();
 exit(0);
 }
 *
 */ 
