/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */

/*
| Outside In Viewer Technology - Include
|
| Include:       VTCHARS.H
| Environment:   Portable
| Function:      Unified character sets for
|                Outside In Technology
*/

#ifndef _VTCHARS_H
#define _VTCHARS_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_VWDEFS_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS      */

#ifdef __cplusplus
extern "C" {
#endif

/*
| Character set is a DWORD where the high word defines the general
| character set and the high byte of the low word is reserved for flags
| and the low byte is the page within the character set.
|  -----------------------------------------------
| | Char Set Identifier  |   Flags   |Page in Set |
|  -----------------------------------------------
*/

/*
|   F l a g s
|
|   NOTE: The SO_UNICODEFONT flag is used to indicate a TrueType font that uses the unicode encoding.  It is NOT part of the
|         character set itself, and thus is masked off when we use it in the character mapping code.
|
|         SO_UNICODEFONT32 indicates a UTF-32 font which we don't currently support.
*/

#define SO_CSSINGLEPAGE      0x00000100L
#define SO_WORDPERFECTFONT   0x00000200L
#define SO_UNICODEFONT       0x00000400L
#define SO_UNICODEFONT32     0x00000800L

#define SO_ANSIMASK          0x10000000L
#define SO_FONTMASK          0x20000000L
#define SO_EBCDICMASK        0x40000000L
#define SO_MACMASK           0x80000000L

#define SO_MAPPROBFLAG       0x00008000L

/*
| This one is for some of the unix dbcs charsets. Since we ran out of upper nibble mask bits, this one must be tested differently than
| a regular mask.  Also, this relies on the fact that as of now there are no other charsets that have an F in that location, so don't make
| one that does in the future!!
*/
#define SO_UNIXUNMASK       0x0F000000L
#define SO_CHARFAMILYMASK   0x000F0000L


/*
|   S p e c i a l
*/

#define SO_DEFAULTCHARSET   0

/*
|   U n k n o w n s
*/

#define SO_PCUNKNOWN        (0x00010000L | SO_CSSINGLEPAGE) /* 0x00010100 */
#define SO_PC               SO_PCUNKNOWN
#define SO_MACUNKNOWN       (0x00020000L | SO_CSSINGLEPAGE) /* 0x00020100 */
#define SO_MAC              SO_MACUNKNOWN
#define SO_ANSIUNKNOWN      (0x00030000L | SO_CSSINGLEPAGE) /* 0x00030100 */
#define SO_WINDOWS          SO_ANSIUNKNOWN
#define SO_DBCSUNKNOWN      (0x00050000L)
#define SO_DBCS             SO_DBCSUNKNOWN

#define SO_WPF5             (0x00060000L)
#define SO_WPF6             (0x00070000L)      /* WordPerfect changed some pages of their character set between 5 & 6 */
#define SO_ISO8859          (0x00080000L)
#define SO_LOTUSMBCS        (0x00090000L)      /* For Lotus 123. */
#define SO_HTMLSBCS         (0x000A0000L)
#define SO_HTMLDBCS         (0x000B0000L)
#define SO_OLD_HEBREW       (0x000C0000L)      /* For BIDI */

/*
|   U N I X  DBCS PAGES, these are all now stored in some form in the bin file instead of being created algorithmically.
*/

#define SOMAKEUNIXDBCS(wCodePage) (SO_UNIXUNMASK | ((VTDWORD)(wCodePage) << 16L))

#define SO_JIS                     SOMAKEUNIXDBCS(0x0C)    /* 0x0F0C0000 for jis character sets, used on unix  jis 0208-1990             */
#define SO_EUC_JP                  SOMAKEUNIXDBCS(0x0D)    /* 0x0F0D0000 euc for jis character sets, it's just jis0208-1990|0x8080       */
#define SO_CNS11643_1              SOMAKEUNIXDBCS(3)       /* 0x0F030000 chinese traditional character set cns11643-1986, plane one      */
#define SO_EUC_CNS_1               SOMAKEUNIXDBCS(7)       /* 0x0F070000 plane 1 of cns11643 in euc , it's just |x8080  -kef             */
#define SO_CNS11643_2              SOMAKEUNIXDBCS(4)       /* 0x0F040000 chinese traditional character set cns11643-1986, plane 2        */
#define SO_EUC_CNS_2               SOMAKEUNIXDBCS(8)       /* 0x0F080000 plane 2 of cns11643 in euc, it's just |x8080 -kef               */
#define SO_KSC1987                 SOMAKEUNIXDBCS(6)       /* 0x0F060000 korean standard ksc1987-1992                                    */
#define SO_GB2312                  SOMAKEUNIXDBCS(5)       /* 0x0F050000 simplified chinese GB2312-80                                    */

#define SO_JIS1978                 SO_JIS                  /* 0x0F0C0000 JIS C 6226-1978 - an early version of JIS X 0208-1990 */
#define SO_JIS1983                 SO_JIS                  /* 0x0F0C0000 JIS X 0208-1983 - an early version of JIS X 0208-1990 */
#define SO_JIS1990                 SO_JIS                  /* 0x0F0C0000 JIS X 0208-1990                                       */

#define SOMAKEWPF5CS(wCodePage) (SO_WPF5 | SO_CSSINGLEPAGE | SO_WORDPERFECTFONT | (VTDWORD)(wCodePage))
	/* 0x0006030 + wCodePage */
#define SOMAKEWPF6CS(wCodePage) (SO_WPF6 | SO_CSSINGLEPAGE | SO_WORDPERFECTFONT | (VTDWORD)(wCodePage))
	/* 0x0007030 + wCodePage */
#define SOMAKELOTUSMBCS(wCodePage) (SO_LOTUSMBCS | SO_CSSINGLEPAGE | (VTDWORD)(wCodePage))
	/* 0x0009010 + wCodePage */

/*
|   EBCDIC pages (and some other IBM pages)
*/

#define SOMAKEEBCDICSBCS(wCodePage) (SO_EBCDICMASK | ((VTDWORD)(wCodePage) << 16L) | SO_CSSINGLEPAGE)
#define SO_EBCDIC37                 SOMAKEEBCDICSBCS(37)    /* 0x40250100 */
#define SO_EBCDIC273                SOMAKEEBCDICSBCS(273)   /* 0x41110100 */
#define SO_EBCDIC274                SOMAKEEBCDICSBCS(274)   /* 0x41120100 */
#define SO_EBCDIC277                SOMAKEEBCDICSBCS(277)   /* 0x41150100 */
#define SO_EBCDIC278                SOMAKEEBCDICSBCS(278)   /* 0x41160100 */
#define SO_EBCDIC280                SOMAKEEBCDICSBCS(280)   /* 0x41180100 */
#define SO_EBCDIC282                SOMAKEEBCDICSBCS(282)   /* 0x411A0100 */
#define SO_EBCDIC284                SOMAKEEBCDICSBCS(284)   /* 0x411C0100 */
#define SO_EBCDIC285                SOMAKEEBCDICSBCS(285)   /* 0x411D0100 */
#define SO_EBCDIC297                SOMAKEEBCDICSBCS(297)   /* 0x41290100 */
#define SO_EBCDIC500                SOMAKEEBCDICSBCS(500)   /* 0x41F40100 */
#define SO_EBCDIC870                SOMAKEEBCDICSBCS(870)   /* 0x43660100 */
#define SO_EBCDIC871                SOMAKEEBCDICSBCS(871)   /* 0x43670100 */
#define SO_EBCDIC1026               SOMAKEEBCDICSBCS(1026)  /* 0x44020100 */

#define SO_KOI8_R                   SOMAKEEBCDICSBCS(878)   /* 0x436E0100 */
#define SO_KOI8_U                   SOMAKEEBCDICSBCS(1168)  /* 0x44900100 */

#define SO_DCA                      SO_EBCDIC37            /* 0x40250100 This is so the DCA WIN.3f filter compile */

/*
|   ANSI pages
*/

/* Some of the ANSI encodings below are actually OEM (DOS) encodings */

#define SOMAKEANSISBCS(wCodePage) (SO_ANSIMASK | ((VTDWORD)(wCodePage) << 16L) | SO_CSSINGLEPAGE)
#define SOMAKEANSIDBCS(wCodePage) (SO_ANSIMASK | ((VTDWORD)(wCodePage) << 16L))
#define SO_ANSI0                    SOMAKEANSISBCS(0)     /* 0x10000100 Ansi 0 is 7-Bit ASCII */
#define SO_ASCII                    SO_ANSI0
#define SO_ANSI437                  SOMAKEANSISBCS(437)   /* 0x11B50100 United States */
#define SO_ANSI708                  SOMAKEANSISBCS(708)   /* 0x12C40100 Arabic */
#define SO_ANSI709                  SOMAKEANSISBCS(709)   /* 0x12C50100 Arabic */
#define SO_ANSI710                  SOMAKEANSISBCS(710)   /* 0x12C60100 Arabic */
#define SO_ANSI720                  SOMAKEANSISBCS(720)   /* 0x12D00100 Arabic */
#define SO_ANSI737                  SOMAKEANSISBCS(737)   /* 0x12E10100 */
#define SO_ANSI775                  SOMAKEANSISBCS(775)   /* 0x13070100 */
#define SO_ANSI850                  SOMAKEANSISBCS(850)   /* 0x13520100 */
#define SO_ANSI852                  SOMAKEANSISBCS(852)   /* 0x13540100 */
#define SO_ANSI855                  SOMAKEANSISBCS(855)   /* 0x13570100 */
#define SO_ANSI857                  SOMAKEANSISBCS(857)   /* 0x13590100 */
#define SO_ANSI860                  SOMAKEANSISBCS(860)   /* 0x135C0100 */
#define SO_ANSI861                  SOMAKEANSISBCS(861)   /* 0x135D0100 */
#define SO_ANSI862                  SOMAKEANSISBCS(862)   /* 0x135E0100 */
#define SO_ANSI863                  SOMAKEANSISBCS(863)   /* 0x135F0100 */
#define SO_ANSI864                  SOMAKEANSISBCS(864)   /* 0x13600100 */
#define SO_ANSI865                  SOMAKEANSISBCS(865)   /* 0x13610100 */
#define SO_ANSI866                  SOMAKEANSISBCS(866)   /* 0x13620100 */
#define SO_ANSI869                  SOMAKEANSISBCS(869)   /* 0x13650100 */
#define SO_ANSI874                  SOMAKEANSISBCS(874)   /* 0x136A0100 */

#define SO_ANSI932                  SOMAKEANSIDBCS(932)    /* 0x13A40000 Japan (see JIS X 208) */
#define SO_ANSI936                  SOMAKEANSIDBCS(936)    /* 0x13A80000 Chinese GB */
#define SO_ANSI942                  SOMAKEANSIDBCS(942)    /* 0x13AE0000 */
#define SO_ANSI944                  SOMAKEANSIDBCS(944)    /* 0x13B00000 */
#define SO_ANSI948                  SOMAKEANSIDBCS(948)    /* 0x13B40000 */
#define SO_ANSI949                  SOMAKEANSIDBCS(949)    /* 0x13B50000 Korean Hangeul */
#define SO_ANSI950                  SOMAKEANSIDBCS(950)    /* 0x13B60000 Chinese Big5 */

#define SO_ANSI1040                 SOMAKEANSISBCS(1040)   /* 0x14100100 */
#define SO_ANSI1041                 SOMAKEANSISBCS(1041)   /* 0x14110100 */
#define SO_ANSI1043                 SOMAKEANSISBCS(1043)   /* 0x14130100 */

#define SO_THAINOVELL               SOMAKEANSISBCS(1044)   /* 0x14140100 Thai page for WP Novell*/

#define SO_ANSI1200                 SOMAKEANSIDBCS(1200)   /* 0x14B00000 */
#define SO_ANSI1201                 SOMAKEANSIDBCS(1201)   /* 0x14B10000 */
#define SO_ANSI1250                 SOMAKEANSISBCS(1250)   /* 0x14E20100 */
#define SO_ANSI1251                 SOMAKEANSISBCS(1251)   /* 0x14E30100 */
#define SO_ANSI1252                 SOMAKEANSISBCS(1252)   /* 0x14E40100 */
#define SO_ANSI1253                 SOMAKEANSISBCS(1253)   /* 0x14E50100 */
#define SO_ANSI1254                 SOMAKEANSISBCS(1254)   /* 0x14E60100 */
#define SO_ANSI1255                 SOMAKEANSISBCS(1255)   /* 0x14E70100 */
#define SO_ANSI1256                 SOMAKEANSISBCS(1256)   /* 0x14E80100 */
#define SO_ANSI1257                 SOMAKEANSISBCS(1257)   /* 0x14E90100 */
#define SO_ANSI1258                 SOMAKEANSISBCS(1258)   /* 0x14EA0100 Windows Vietnamese ANSI/OEM 1258 */

#define SO_HWP_HANGUL               SOMAKEANSIDBCS(1260)   /* 0x14EC0000 */
#define SO_ANSI1361                 SOMAKEANSIDBCS(1361)   /* 0x15510000 Korean Johab */

/* Unicode and Ansi 1200 are the same  */
#define SO_UNICODE                  SO_ANSI1200            /* 0x14B00000 */
#define SOMAKEUNICODEPAGECS(wCodePage) (SO_UNICODE | (VTDWORD)(wCodePage))

/* 
 | Unknown Unicode is used for cases where we have a very old TTF font, and the Unicode support is not
 | enumerated.
 */
#define SO_UNKNOWN_UNICODE          SO_ANSI1201            /* 0x14B10000 */

/*
 |
 | These two are for use with the default input character set feature, to allow support for unicode files
 | without an appropriate BOM.
 |
 */

#define SO_BIGENDIAN_UNICODE		SOMAKEUNICODEPAGECS(1)	/* 0x14B00001 */
#define SO_LITTLEENDIAN_UNICODE		SOMAKEUNICODEPAGECS(2)  /* 0x14B00002 */


/* Character sets for PDF CID Fonts */
#define SO_PDFCID_JAPAN1_H          SOMAKEANSIDBCS(2001)  /* 0x17d10000 */
#define SO_PDFCID_JAPAN1_V          SOMAKEANSIDBCS(2002)  /* 0x17d20000 */
#define SO_PDFCID_JAPAN2            SOMAKEANSIDBCS(2003)  /* 0x17d30000 */
#define SO_PDFCID_GB1               SOMAKEANSIDBCS(2004)  /* 0x17d40000 */
#define SO_PDFCID_CNS_H             SOMAKEANSIDBCS(2005)  /* 0x17d50000 */
#define SO_PDFCID_CNS_V             SOMAKEANSIDBCS(2006)  /* 0x17d60000 */
#define SO_PDFCID_KOREA1            SOMAKEANSIDBCS(2007)  /* 0x17d70000 */


/*
|   ISO pages
*/

#define SOMAKEISO8859(wCodePage) (SO_ISO8859 | SO_CSSINGLEPAGE | (VTDWORD)(wCodePage))
#define SO_ISO8859_1                SOMAKEISO8859(1)   /* 0x00080101 Latin-1 - subset of ANSI 1252 */
#define SO_ISO8859_2                SOMAKEISO8859(2)   /* 0x00080102 Latin-2                       */
#define SO_ISO8859_3                SOMAKEISO8859(3)   /* 0x00080103 Latin-3                       */
#define SO_ISO8859_4                SOMAKEISO8859(4)   /* 0x00080104 Latin-4                       */
#define SO_ISO8859_5                SOMAKEISO8859(5)   /* 0x00080105 Cyrillic                      */
#define SO_ISO8859_6                SOMAKEISO8859(6)   /* 0x00080106 Arabic                        */
#define SO_ISO8859_7                SOMAKEISO8859(7)   /* 0x00080107 Greek - subset of ANSI 1253   */
#define SO_ISO8859_8                SOMAKEISO8859(8)   /* 0x00080108 Hebrew                        */
#define SO_ISO8859_9                SOMAKEISO8859(9)   /* 0x00080109 Turkish                       */
#define SO_ISO8859_10               SOMAKEISO8859(10)  /* 0x0008010A Latin-6                       */
#define SO_ISO8859_11               SOMAKEISO8859(11)  /* 0x0008010B Part 11: Latin/Thai alphabet  */
#define SO_ISO8859_12               SOMAKEISO8859(12)  /* 0x0008010C Abandoned                     */
#define SO_ISO8859_13               SOMAKEISO8859(13)  /* 0x0008010D Latin-7                       */
#define SO_ISO8859_14               SOMAKEISO8859(14)  /* 0x0008010E Latin-8                       */
#define SO_ISO8859_15               SOMAKEISO8859(15)  /* 0x0008010F Latin-9                       */
#define SO_ISO8859_16               SOMAKEISO8859(16)  /* 0x00080110 Latin-10                      */

/*
|   Mac pages
*/

#define SOMAKEMACSBCS(wCodePage) (SO_MACMASK | ((VTDWORD)(wCodePage) << 16L) | SO_CSSINGLEPAGE)
#define SOMAKEMACDBCS(wCodePage) (SO_MACMASK | ((VTDWORD)(wCodePage) << 16L) )
#define SO_MACROMAN                 SOMAKEMACSBCS(0)    /* 0x80000100 */
#define SO_MACROMANCROATIAN         SOMAKEMACSBCS(1)    /* 0x80010100 */
#define SO_MACROMANROMANIAN         SOMAKEMACSBCS(2)    /* 0x80020100 */
#define SO_MACROMANTURKISH          SOMAKEMACSBCS(3)    /* 0x80030100 */
#define SO_MACROMANICELANDIC        SOMAKEMACSBCS(4)    /* 0x80040100 */
#define SO_MACCYRILLIC              SOMAKEMACSBCS(5)    /* 0x80050100 */
#define SO_MACGREEK                 SOMAKEMACSBCS(6)    /* 0x80060100 */
#define SO_MACLATIN2                SOMAKEMACSBCS(7)    /* 0x80070100 */
#define SO_GREEK2                   SOMAKEMACSBCS(8)    /* 0x80080100 */
#define SO_HEBREW                   SOMAKEMACSBCS(9)    /* 0x80090100 */
#define SO_ARABIC                   SOMAKEMACSBCS(10)   /* 0x800A0100 */
#define SO_MACJIS                   SOMAKEMACDBCS(11)   /* 0x800B0000 */

/*
|   Font specific pages
*/

#define SOMAKEFONTCS(wIndex) (SO_FONTMASK | SO_CSSINGLEPAGE | ((VTDWORD)(wIndex) << 16))
#define SO_WINSYMBOL                 SOMAKEFONTCS(0)    /* 0x20000100 */
#define SO_MACSYMBOL                 SOMAKEFONTCS(1)    /* 0x20010100 */
#define SO_PLACEHOLDER               SOMAKEFONTCS(2)    /* 0x20020100 THIS SPACE IS AVAILABLE */
#define SO_MSLINEDRAW                SOMAKEFONTCS(3)    /* 0x20030100 */
#define SO_ZAPFDINGBATS              SOMAKEFONTCS(4)    /* 0x20040100 */
#define SO_WPARABIC                  SOMAKEFONTCS(5)    /* 0x20050100 */
#define SO_WPARABICSCRIPT            SOMAKEFONTCS(6)    /* 0x20060100 */
#define SO_WPBOXDRAWING              SOMAKEFONTCS(7)    /* 0x20070100 */
#define SO_WPCYRILLICA               SOMAKEFONTCS(8)    /* 0x20080100 */
#define SO_WPCYRILLICB               SOMAKEFONTCS(9)    /* 0x20090100 */
#define SO_WPGREEK                   SOMAKEFONTCS(10)   /* 0x200A0100 */
#define SO_WPHEBREWDAVID             SOMAKEFONTCS(11)   /* 0x200B0100 */
#define SO_WPICONICSYMBOLSA          SOMAKEFONTCS(12)   /* 0x200C0100 */
#define SO_WPICONICSYMBOLSB          SOMAKEFONTCS(13)   /* 0x200D0100 */
#define SO_WPJAPANESE                SOMAKEFONTCS(14)   /* 0x200E0100 */
#define SO_WPMATHA                   SOMAKEFONTCS(15)   /* 0x200F0100 */
#define SO_WPMATHB                   SOMAKEFONTCS(16)   /* 0x20100100 */
#define SO_WPEXTENDEDMATHA           SOMAKEFONTCS(17)   /* 0x20110100 */
#define SO_WPEXTENDEDMATHB           SOMAKEFONTCS(18)   /* 0x20120100 */
#define SO_WPMULTINATIONALA          SOMAKEFONTCS(19)   /* 0x20130100 */
#define SO_WPMULTINATIONALB          SOMAKEFONTCS(20)   /* 0x20140100 */
#define SO_WPOVERFLOWSET             SOMAKEFONTCS(21)   /* 0x20150100 */
#define SO_WPPHONETIC                SOMAKEFONTCS(22)   /* 0x20160100 */
#define SO_WPTYPOGRAPHIC             SOMAKEFONTCS(23)   /* 0x20170100 */
#define SO_MTEXTRA                   SOMAKEFONTCS(24)   /* 0x20180100 */
#define SO_BOOKSHELFSYMBOL3          SOMAKEFONTCS(25)   /* 0x20190100 */
#define SO_HPROMAN8                  SOMAKEFONTCS(26)   /* 0x201A0100 */
#define SO_MICROSOFTOFFICEPRIVATEUSE SOMAKEFONTCS(27)   /* 0x201B0100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_WINWEBDINGS               SOMAKEFONTCS(28)   /* 0x201C0100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_WINWINGDINGS              SOMAKEFONTCS(29)   /* 0x201D0100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_WINWINGDINGS2             SOMAKEFONTCS(30)   /* 0x201E0100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_WINWINGDINGS3             SOMAKEFONTCS(31)   /* 0x201F0100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_BOTANICAL                 SOMAKEFONTCS(32)   /* 0x20200100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_MONOTYPESORTS             SOMAKEFONTCS(33)   /* 0x20210100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_BOOKSHELFSYMBOL7          SOMAKEFONTCS(34)   /* 0x20220100 NOTE:  No corresponding entry in cmmap000.bin */
#define SO_GENERICSYMBOL             SOMAKEFONTCS(35)   /* 0x20230100 NOTE:  No corresponding entry in cmmap000.bin */

/*
| WARNING:
| The above SO_ charset definitions cannot be changed.  Character Map bin file is created
| based on this order, additional charsets may only be added at the end
*/

/*
|   Bidi mappings
*/

#define SOMAKEBIDI(num)	(SO_OLD_HEBREW | SO_CSSINGLEPAGE | (VTDWORD)(num))
#define SO_BIDI_OLDCODE             SOMAKEBIDI(1)   /* 0x000C0101 */
#define SO_BIDI_PC8                 SOMAKEBIDI(2)   /* 0x000C0102 */
#define SO_BIDI_E0                  SOMAKEBIDI(3)   /* 0x000C0103 */

/*
| HTML Pages
*/

#define SOMAKEHTMLSBCS(wCodePage) (SO_HTMLSBCS | SO_CSSINGLEPAGE | (VTDWORD)(wCodePage))
#define SOMAKEHTMLDBCS(wCodePage) (SO_HTMLDBCS | (VTDWORD)(wCodePage))
#define SO_HTMLKOI8                 SOMAKEHTMLSBCS(1)   /* 0x000A0101 Russian - koi8-r is currently not an international standard       */
#define SO_JIS_ROMAN                SOMAKEHTMLSBCS(2)   /* 0x000A0102 JIS X 0201-1976 - very similar to 7 bit ASCII; the "first side"   */
#define SO_ISO646                   SOMAKEHTMLSBCS(3)   /* 0x000A0103 Very similar to 7 bit, no contol char ASCII                       */
#define SO_JIS_KANA                 SOMAKEHTMLSBCS(4)   /* 0x000A0104 JIS X 0201-1976 - Kana characters (half-width); the "second side" */

/* we don't support these upper planes of cns at this time */
#define SO_CNS11643_3               SOMAKEHTMLDBCS(6)   /* 0x000B0006 CSN11643.1992-3 - traditional Chinese, plane 3 */
#define SO_CNS11643_4               SOMAKEHTMLDBCS(7)   /* 0x000B0007 CSN11643.1992-4 - traditional Chinese, plane 4 */
#define SO_CNS11643_5               SOMAKEHTMLDBCS(8)   /* 0x000B0008 CSN11643.1992-5 - traditional Chinese, plane 5 */
#define SO_CNS11643_6               SOMAKEHTMLDBCS(9)   /* 0x000B0009 CSN11643.1992-6 - traditional Chinese, plane 6 */
#define SO_CNS11643_7               SOMAKEHTMLDBCS(10)  /* 0x000B000A CSN11643.1992-7 - traditional Chinese, plane 7 */


/*
| UTF-8 support is only used by HTML Export right now, so the definition of it doesn't
| really matter.  If the VT ever attempts to handle UTF-8 input, this #define may need to change
| but we'll worry about it then. And UTF-7 by the MIME filter.  SO_ISO2022JP, SO_ISO2022KR,
| SO_HZGB2312 are encodings that were added for the the MSG, MIME and HTML filters.  They are not
| actual character sets.
*/
#define SO_UTF8                     SOMAKEHTMLDBCS(11)  /* 0x000B000B */
#define SO_UTF7                     SOMAKEHTMLDBCS(12)  /* 0x000B000C */
#define SO_ISO2022JP				SOMAKEHTMLDBCS(13)  /* 0x000B000D */
#define SO_ISO2022KR				SOMAKEHTMLDBCS(14)  /* 0x000B000E */
#define SO_HZGB2312                 SOMAKEHTMLDBCS(15)  /* 0x000B000F */

/* 
 | The following are Unicode ranges as defined in the OpenType font specification document.  The ranges are defined
 | as part of the OS/2 table in OpenType/TrueType fonts.
 */

#define SO_MAKE_UNICODERANGE_SINGLEPAGE( page ) ( SO_MACMASK | ( (VTDWORD)( page ) << 16L ) | SO_CSSINGLEPAGE )
#define SO_MAKE_UNICODERANGE_MULTIPAGE( page ) ( SO_MACMASK | ( (VTDWORD)( page ) << 16L ) )

#define SO_UR_BASIC_LATIN                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0100 )
#define SO_UR_LATIN_1_SUPPLEMENT                      SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0101 )
#define SO_UR_LATIN_EXTENDED_A                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0102 )
#define SO_UR_LATIN_EXTENDED_B                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0103 ) 
#define SO_UR_IPA_EXTENSIONS                          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0104 )
#define SO_UR_PHONETIC_EXTENSIONS                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0105 )
#define SO_UR_PHONETIC_EXTENSIONS_SUPPLEMENT          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0106 )
#define SO_UR_SPACING_MODIFIER_LETTERS                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0107 )
#define SO_UR_MODIFIER_TONE_LETTERS                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0108 )
#define SO_UR_COMBINING_DIACRITICAL_MARKS             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0109 )
#define SO_UR_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010A )
#define SO_UR_GREEK_AND_COPTIC                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010B )
#define SO_UR_COPTIC                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010C )
#define SO_UR_CYRILLIC                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010D )
#define SO_UR_CYRILLIC_SUPPLEMENT                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010E )
#define SO_UR_CYRILLIC_EXTENDED_A                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x010F )
#define SO_UR_CYRILLIC_EXTENDED_B                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0110 )
#define SO_UR_ARMENIAN                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0111 )
#define SO_UR_HEBREW                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0112 )
#define SO_UR_VAI                                     SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0113 )
#define SO_UR_ARABIC                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0114 )
#define SO_UR_ARABIC_SUPPLEMENT                       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0115 )
#define SO_UR_NKO                                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0116 )
#define SO_UR_DEVANAGARI                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0117 )
#define SO_UR_BENGALI                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0118 )
#define SO_UR_GURMUKHI                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0119 )
#define SO_UR_GUJARATI                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011A )
#define SO_UR_ORIYA                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011B )
#define SO_UR_TAMIL                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011C )
#define SO_UR_TELUGU                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011D )
#define SO_UR_KANNADA                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011E )
#define SO_UR_MALAYALAM                               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x011F )
#define SO_UR_THAI                                    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0120 )
#define SO_UR_LAO                                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0121 )
#define SO_UR_GEORGIAN                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0122 )
#define SO_UR_GEORGIAN_SUPPLEMENT                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0123 )
#define SO_UR_BALINESE                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0124 )
#define SO_UR_HANGUL_JAMO                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0125 )
#define SO_UR_LATIN_EXTENDED_ADDITIONAL               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0126 )
#define SO_UR_LATIN_EXTENDED_C                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0127 )
#define SO_UR_LATIN_EXTENDED_D                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0128 )
#define SO_UR_GREEK_EXTENDED                          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0129 )
#define SO_UR_GENERAL_PUNCTUATION                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012A )
#define SO_UR_SUPPLEMENTAL_PUNCTUATION                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012B )
#define SO_UR_SUPERSCRIPTS_AND_SUBSCRIPTS             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012C )
#define SO_UR_CURRENCY_SYMBOLS                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012D )
#define SO_UR_COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012E )
#define SO_UR_LETTERLIKE_SYMBOLS                      SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x012F )
#define SO_UR_NUMBER_FORMS                            SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0130 )
#define SO_UR_ARROWS                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0131 )
#define SO_UR_SUPPLEMENTAL_ARROWS_A                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0132 )
#define SO_UR_SUPPLEMENTAL_ARROWS_B                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0133 )
#define SO_UR_MISCELLANEOUS_SYMBOLS_AND_ARROWS        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0134 )
#define SO_UR_MATHEMATICAL_OPERATORS                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0135 )
#define SO_UR_SUPPLEMENTAL_MATHEMATICAL_OPERATORS     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0136 )
#define SO_UR_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0137 )
#define SO_UR_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0138 )
#define SO_UR_MISCELLANEOUS_TECHNICAL                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0139 )
#define SO_UR_CONTROL_PICTURES                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013A )
#define SO_UR_OPTICAL_CHARACTER_RECOGNITION           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013B )
#define SO_UR_ENCLOSED_ALPHANUMERICS                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013C )
#define SO_UR_BOX_DRAWING                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013D )
#define SO_UR_BLOCK_ELEMENTS                          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013E )
#define SO_UR_GEOMETRIC_SHAPES                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x013F )
#define SO_UR_MISCELLANEOUS_SYMBOLS                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0140 )
#define SO_UR_DINGBATS                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0141 )
#define SO_UR_CJK_SYMBOLS_AND_PUNCTUATION             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0142 )
#define SO_UR_HIRAGANA                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0143 )
#define SO_UR_KATAKANA                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0144 )
#define SO_UR_KATAKANA_PHONETIC_EXTENSIONS            SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0145 )
#define SO_UR_BOPOMOFO                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0146 )
#define SO_UR_BOPOMOFO_EXTENDED                       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0147 )
#define SO_UR_HANGUL_COMPATIBILITY_JAMO               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0148 )
#define SO_UR_PHAGS_PA                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0149 )
#define SO_UR_ENCLOSED_CJK_LETTERS_AND_MONTHS         SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x014A )
#define SO_UR_CJK_COMPATIBILITY                       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x014B )
#define SO_UR_HANGUL_SYLLABLES                        SO_MAKE_UNICODERANGE_MULTIPAGE( 0x014C )
#define SO_UR_NON_PLANE_0                             SO_MAKE_UNICODERANGE_MULTIPAGE( 0x014D )
#define SO_UR_PHOENICIAN                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x014E )   /* NOTE: Not currently defined */
#define SO_UR_CJK_UNIFIED_IDEOGRAPHS                  SO_MAKE_UNICODERANGE_MULTIPAGE( 0x014F )
#define SO_UR_CJK_RADICALS_SUPPLEMENT                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0150 )
#define SO_UR_KANGXI_RADICALS                         SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0151 )
#define SO_UR_IDEOGRAPHIC_DESCRIPTION_CHARACTERS      SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0152 )
#define SO_UR_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A      SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0153 )
#define SO_UR_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B      SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0154 )    /* NOTE: Not currently defined */
#define SO_UR_KANBUN                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0155 )
#define SO_UR_CJK_STROKES                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0156 )
#define SO_UR_CJK_COMPATIBILITY_IDEOGRAPHS            SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0157 )
#define SO_UR_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0158 )    /* NOTE: Not currently defined */
#define SO_UR_ALPHABETIC_PRESENTATION_FORMS           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0159 )
#define SO_UR_ARABIC_PRESENTATION_FORMS_A             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015A )
#define SO_UR_COMBINING_HALF_MARKS                    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015B )
#define SO_UR_VERTICAL_FORMS                          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015C )
#define SO_UR_CJK_COMPATIBILITY_FORMS                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015D )
#define SO_UR_SMALL_FORM_VARIANTS                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015E )
#define SO_UR_ARABIC_PRESENTATION_FORMS_B             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x015F )
#define SO_UR_HALFWIDTH_AND_FULLWIDTH_FORMS           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0160 )
#define SO_UR_SPECIALS                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0161 )
#define SO_UR_TIBETAN                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0162 )
#define SO_UR_SYRIAC                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0163 )
#define SO_UR_THAANA                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0164 )
#define SO_UR_SINHALA                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0165 )
#define SO_UR_MYANMAR                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0166 )
#define SO_UR_ETHIOPIC                                SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0167 )
#define SO_UR_ETHIOPIC_SUPPLEMENT                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0168 )
#define SO_UR_ETHIOPIC_EXTENDED                       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0169 )
#define SO_UR_CHEROKEE                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x016A )
#define SO_UR_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS   SO_MAKE_UNICODERANGE_MULTIPAGE( 0x016B )
#define SO_UR_OGHAM                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x016C )
#define SO_UR_RUNIC                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x016D )
#define SO_UR_KHMER                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x016E )
#define SO_UR_KHMER_SYMBOLS                           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x016F )
#define SO_UR_MONGOLIAN                               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0170 )
#define SO_UR_BRAILLE_PATTERNS                        SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0171 )
#define SO_UR_YI_SYLLABLES                            SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0172 )
#define SO_UR_YI_RADICALS                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0173 )
#define SO_UR_TAGALOG                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0174 )
#define SO_UR_HANUNOO                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0175 )
#define SO_UR_BUHID                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0176 )
#define SO_UR_TAGBANWA                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0177 )
#define SO_UR_OLD_ITALIC                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0178 )   /* NOTE: Not currently defined */
#define SO_UR_GOTHIC                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0179 )   /* NOTE: Not currently defined */
#define SO_UR_DESERET                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x017A )   /* NOTE: Not currently defined */
#define SO_UR_BYZANTINE_MUSICAL_SYMBOLS               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x017B )   /* NOTE: Not currently defined */
#define SO_UR_MUSICAL_SYMBOLS                         SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x017C )   /* NOTE: Not currently defined */
#define SO_UR_ANCIENT_GREEK_MUSICAL_NOTATION          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x017D )   /* NOTE: Not currently defined */
#define SO_UR_MATHEMATICAL_ALPHANUMERIC_SYMBOLS       SO_MAKE_UNICODERANGE_MULTIPAGE( 0x017E )    /* NOTE: Not currently defined */
#define SO_UR_VARIATION_SELECTORS                     SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x017F )
#define SO_UR_VARIATION_SELECTORS_SUPPLEMENT          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0180 )   /* NOTE: Not currently defined */
#define SO_UR_TAGS                                    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0181 )   /* NOTE: Not currently defined */
#define SO_UR_LIMBU                                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0182 )
#define SO_UR_TAI_LE                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0183 )
#define SO_UR_NEW_TAI_LUE                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0184 )
#define SO_UR_BUGINESE                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0185 )
#define SO_UR_GLAGOLITIC                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0186 )
#define SO_UR_TIFINAGH                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0187 )
#define SO_UR_YIJING_HEXAGRAM_SYMBOLS                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0188 )
#define SO_UR_SYLOTI_NAGRI                            SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0189 )
#define SO_UR_LINEAR_B_SYLLABARY                      SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018A )   /* NOTE: Not currently defined */
#define SO_UR_LINEAR_B_IDEOGRAMS                      SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018B )   /* NOTE: Not currently defined */
#define SO_UR_AEGEAN_NUMBERS                          SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018C )   /* NOTE: Not currently defined */
#define SO_UR_ANCIENT_GREEK_NUMBERS                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018D )   /* NOTE: Not currently defined */
#define SO_UR_UGARITIC                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018E )   /* NOTE: Not currently defined */
#define SO_UR_OLD_PERSIAN                             SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x018F )   /* NOTE: Not currently defined */
#define SO_UR_SHAVIAN                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0190 )   /* NOTE: Not currently defined */
#define SO_UR_OSMANYA                                 SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0191 )   /* NOTE: Not currently defined */
#define SO_UR_CYPRIOT_SYLLABARY                       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0192 )   /* NOTE: Not currently defined */
#define SO_UR_KHAROSHTHI                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0193 )   /* NOTE: Not currently defined */
#define SO_UR_TAI_XUAN_JING_SYMBOLS                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0194 )   /* NOTE: Not currently defined */
#define SO_UR_CUNEIFORM                               SO_MAKE_UNICODERANGE_MULTIPAGE( 0x0195 )    /* NOTE: Not currently defined */
#define SO_UR_CUNEIFORM_NUMBERS_AND_PUNCTUATION       SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0196 )   /* NOTE: Not currently defined */
#define SO_UR_COUNTING_ROD_NUMERALS                   SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0197 )   /* NOTE: Not currently defined */
#define SO_UR_SUNDANESE                               SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0198 )
#define SO_UR_LEPCHA                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x0199 )
#define SO_UR_OL_CHIKI                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019A )
#define SO_UR_SAURASHTRA                              SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019B )
#define SO_UR_KAYAH_LI                                SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019C )
#define SO_UR_REJANG                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019D )
#define SO_UR_CHAM                                    SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019E )
#define SO_UR_ANCIENT_SYMBOLS                         SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x019F )   /* NOTE: Not currently defined */
#define SO_UR_PHAISTOS_DISC                           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A0 )   /* NOTE: Not currently defined */
#define SO_UR_CARIAN                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A1 )   /* NOTE: Not currently defined */
#define SO_UR_LYCIAN                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A2 )   /* NOTE: Not currently defined */
#define SO_UR_LYDIAN                                  SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A3 )   /* NOTE: Not currently defined */
#define SO_UR_DOMINO_TILES                            SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A4 )   /* NOTE: Not currently defined */
#define SO_UR_MAHJONG_TILES                           SO_MAKE_UNICODERANGE_SINGLEPAGE( 0x01A5 )   /* NOTE: Not currently defined */
#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_VWDEFS_H
#pragma pack(pop)
#undef SCC_PACKED_BY_VWDEFS_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_VWDEFS_H */

#endif /* _VTCHARS_H */
