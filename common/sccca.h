/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved.*/
/*
|   Content Access
|   Include File sccca.h
|
|    CCC     A
|   C   C   A A
|   C      A   A
|   C      AAAAA
|   C   C  A   A
|    CCC   A   A
|
*/

#ifndef _SCCCA_H
#define _SCCCA_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCCA_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#ifndef RC_INVOKED
#include "scctype.h"
#include <sccda.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

/*
|  T Y P E D E F S
*/

typedef VTHDOC   VTHCONTENT;
typedef VTLPHDOC VTLPHCONTENT;
#define CA_INVALIDITEM  ((VTDWORD)-1)


/*
|  S T R U C T U R E S
*/


typedef struct tagSCCCAGETCONTENT
{
    VTDWORD  dwStructSize;
    VTDWORD  dwFlags;
    VTDWORD  dwMaxBufSize;
    VTDWORD  dwType;
    VTDWORD  dwSubType;
    VTDWORD  dwData1;
    VTDWORD  dwData2;
    VTDWORD  dwData3;
    VTDWORD  dwData4;
    VTDWORD  dwDataBufSize;
    VTVOID *pDataBuf;
} SCCCAGETCONTENT, *PSCCCAGETCONTENT;

/* Possible values for SCCCAGETCONTENT.dwType */
#define  SCCCA_BEGINSECTION_TYPE "Type"

#define  SCCCA_TEXT                 0x0001
#define  SCCCA_BEGINTAG             0x0002
#define  SCCCA_ENDTAG               0x0003
#define  SCCCA_FILEPROPERTY         0x0004
#define  SCCCA_BREAK                0x0005
#define  SCCCA_OBJECT               0x0006
#define  SCCCA_STYLECHANGE          0x0007
#define  SCCCA_GENERATED            0x0008
#define  SCCCA_SHEET                0x0009
#define  SCCCA_ANNOTATION           0x000a
#define  SCCCA_TREENODELOCATOR      0x000b
#define  SCCCA_COMMENTREFERENCE     0x000c
#define  SCCCA_CELL                 0x000d
#define  SCCCA_RECORD               0x000e
#define  SCCCA_SLIDE                0x000f
#define  SCCCA_OBJECTNAME           0x0010
#define  SCCCA_OBJECTALTSTRING      0x0011
#define  SCCCA_REVISION_CELL		0X0012
#define  SCCCA_REVISION_ROW			0X0013
#define  SCCCA_REVISION_COLUMN		0X0014
#define  SCCCA_REVISION_SHEET		0X0015
#define  SCCCA_REVISION_USER		0X0016
#define  SCCCA_REVISION_SHEETNAME	0X0017

#define  SCCCA_ENDSECTION_TYPE

/* If dwType equals SCCCA_SLIDE then dwFlags can be ... */ 
#define  SCCCA_BEGINSECTION_SLIDETYPE "SlideType"

#define  SCCCA_SLIDENORMAL            0x0000
#define  SCCCA_SLIDEHIDDEN            0x0001

#define  SCCCA_ENDSECTION_SLIDETYPE

/* If dwType equals SCCCA_OBJECT then dwSubType can be ... */
#define  SCCCA_BEGINSECTION_OBJECTSUBTYPE "ObjectSubtype"

#define  SCCCA_EMBEDDEDOBJECT       0x00000000
#define  SCCCA_ARCHIVEITEMCONTAINER 0x00010000 /*Used for Folder nodes*/
#define  SCCCA_COMPRESSEDFILE       0x00020000 /*Compressed File or the filter does not define node types*/
#define  SCCCA_MESSAGE              0x00030000
#define  SCCCA_CONTACT              0x00040000
#define  SCCCA_CALENDARENTRY        0x00050000
#define  SCCCA_NOTE                 0x00060000
#define  SCCCA_TASK                 0x00070000
#define  SCCCA_JOURNALENTRY         0x00080000
#define  SCCCA_ATTACHMENT           0x00090000
#define  SCCCA_FIELDSFILE           0x000A0000

#define  SCCCA_ENDSECTION_OBJECTSUBTYPE


/* If dwType equals SCCCA_OBJECT then dwFlags can be ... */ 
#define  SCCCA_BEGINSECTION_OBJECTDATA1 "ObjectData1"

#define  SCCCA_ENDRECORD            0x0001

#define  SCCCA_ENDSECTION_OBJECTDATA1


/* If dwType equals SCCCA_TEXT or SCCCA_GENERATED then dwSubType can be ... */
#define  SCCCA_BEGINSECTION_TEXTSUBTYPE "TextSubtype"

#define  SCCCA_DOCUMENTTEXT         0x00000001
#define  SCCCA_SPECIALTEXT          0x00000002
#define  SCCCA_OCRTEXT              0x00000004	/* Not currently implemented */
#define  SCCCA_XMPMETADATA          0x00000008 
#define  SCCCA_URLTEXT              0x00000010 
#define  SCCCA_BOOKMARKTEXT         0x00000020
#define  SCCCA_PLACEHOLDERTEXT      0x00000040

#define  SCCCA_ENDSECTION_TEXTSUBTYPE


/* High Word Masks for dwSubType */
#define  SCCCA_BEGINSECTION_TEXTMASK "TextMask"

#define  SCCCA_REVISIONDELETE       0x00010000
#define  SCCCA_BOLD                 0x00020000
#define  SCCCA_ITALIC               0x00040000
#define  SCCCA_UNDERLINE            0x00080000
#define  SCCCA_DUNDERLINE           0x00100000
#define  SCCCA_OUTLINE              0x00200000
#define  SCCCA_HIDDEN               0x00400000
#define  SCCCA_STRIKEOUT            0x00800000
#define  SCCCA_SMALLCAPS            0x01000000
#define  SCCCA_ALLCAPS              0x02000000
#define  SCCCA_OCEMASK              0x04000000
#define  SCCCA_UNKNOWNMAP           0x08000000
#define  SCCCA_REVISIONADD          0x10000000
#define  SCCCA_ANYHIDDEN    (SCCCA_HIDDEN | SCCCA_REVISIONDELETE)
#define  SCCCA_ANYUNDERLINE (SCCCA_UNDERLINE | SCCCA_DUNDERLINE)
#define  SCCCA_ALLCHARATTRS (SCCCA_BOLD | SCCCA_ITALIC | SCCCA_UNDERLINE | SCCCA_DUNDERLINE | SCCCA_OUTLINE | SCCCA_HIDDEN | SCCCA_STRIKEOUT | SCCCA_SMALLCAPS | SCCCA_ALLCAPS | SCCCA_OCEMASK | SCCCA_REVISIONDELETE | SCCCA_REVISIONADD)

#define  SCCCA_ENDSECTION_TEXTMASK


/* If dwType equals SCCCA_BEGINTAG or SCCCA_ENDTAG then dwSubType can ... */
/*
    Anything that gets added here should have a corresponding string added in
    exd_ixml.c.  TAGNAMETBLSIZE will need to be changed in ex_main.h.  The
    SearchML schema will also have to change.
*/
#define  SCCCA_BEGINSECTION_TAGSUBTYPE "TagSubtype"

#define  SCCCA_DOCUMENTPROPERTY     0x00010000
#define  SCCCA_BOOKMARK             0x00020000
#define  SCCCA_HYPERLINK            0x00030000
#define  SCCCA_COMPILEDFIELD        0x00040000
#define  SCCCA_FOOTNOTEREFERENCE    0x00050000
#define  SCCCA_ANNOTATIONREFERENCE  0x00060000
#define  SCCCA_ENDNOTEREFERENCE     0x00070000
#define  SCCCA_STYLE                0x00080000
#define  SCCCA_FRAME                0x00090000
#define  SCCCA_SUBDOCTEXT           0x000a0000
#define  SCCCA_MERGEENTRY           0x000b0000
#define  SCCCA_INDEXENTRY           0x000c0000
#define  SCCCA_LISTENTRY            0x000d0000
#define  SCCCA_DATEDEFINITION       0x000e0000
#define  SCCCA_CUSTOMDATAFORMAT     0x00100000
#define  SCCCA_NAMEDCELLRANGE       0x00110000
#define  SCCCA_COUNTERFORMAT        0x00120000
#define  SCCCA_INLINEDATAFORMAT     0x00130000
#define  SCCCA_CAPTIONTEXT          0x00140000
#define  SCCCA_VECTORSAVETAG        0x00150000
#define  SCCCA_TOCENTRY             0x00160000
#define  SCCCA_TOC                  0x00170000
#define  SCCCA_INDEX                0x00180000
#define  SCCCA_TOF                  0x00190000
#define  SCCCA_TOA                  0x001a0000
#define  SCCCA_TOAENTRY             0x001b0000
#define  SCCCA_DOCUMENTPROPERTYNAME 0x001c0000
#define  SCCCA_XREF                 0x001d0000
#define  SCCCA_GENERATOR            0x001e0000
#define  SCCCA_CHARACTER            0x00200000
#define  SCCCA_GENERATEDFIELD       0x00210000
#define  SCCCA_ALTFONTDATA          0x00220000
#define  SCCCA_FONTANDGLYPHDATA     0x00230000
#define  SCCCA_REFERENCEDTEXT       0x00240000
#define  SCCCA_SSHEADERFOOTER       0x00250000
#define  SCCCA_FRAME_EX             0x00260000
#define  SCCCA_SLIDENOTES           0x00270000
#define  SCCCA_LINKEDOBJECT         0x00280000
#define  SCCCA_EMAILFIELD           0x00290000
#define  SCCCA_EMAILTABLE           0x002a0000
#define  SCCCA_INTERNALLINK         0x002b0000  /* Not currently implemented */
#define  SCCCA_DIAGRAM              0x002c0000
#define  SCCCA_DIAGRAM_LAYOUT       0x002d0000
#define  SCCCA_DIAGRAM_NODE         0x002e0000
#define  SCCCA_DIAGRAM_COLORSTYLE   0x00300000
#define  SCCCA_DIAGRAM_QUICKSTYLE   0x00310000
#define  SCCCA_DIAGRAM_COLORSTYLE_FILL  0x00320000
#define  SCCCA_DIAGRAM_COLORSTYLE_LINE  0x00330000
#define  SCCCA_DIAGRAM_BULLET       0x00340000
#define  SCCCA_DIAGRAM_LAYOUTNODE   0x00350000
#define  SCCCA_DIAGRAM_ALGORITHM    0x00360000
#define  SCCCA_DIAGRAM_CHOOSE       0x00370000
#define  SCCCA_DIAGRAM_IF           0x00380000
#define  SCCCA_DIAGRAM_ELSE         0x00390000
#define  SCCCA_DIAGRAM_FOREACH      0x003a0000
#define  SCCCA_DIAGRAM_CONSTRAINT   0x003b0000
#define  SCCCA_DIAGRAM_RULE         0x003c0000
#define  SCCCA_DIAGRAM_PRESOF       0x003d0000
#define  SCCCA_DIAGRAM_SHAPE        0x003e0000
#define  SCCCA_CONDITIONALSTYLE     0x003f0000
#define  SCCCA_XMPDATA              0x00400000
#define  SCCCA_RENDEREFFECTS        0x00410000
#define  SCCCA_EMAILFIELDNAME       0x00420000
#define  SCCCA_SUBDOCPROPERTY       0x00430000
#define  SCCCA_HEADER               0x00440000
#define  SCCCA_FOOTER               0x00450000
#define  SCCCA_SS_REVISIONS         0x00460000
#define  SCCCA_SS_USERNAMES         0x00470000
#define  SCCCA_SS_REV_CREATE        0x00480000
#define  SCCCA_SS_REV_SAVE          0x00490000
#define  SCCCA_SS_REV_ADDROW        0x004a0000
#define  SCCCA_SS_REV_DELETEROW     0x004b0000
#define  SCCCA_SS_REV_INSERTCOL     0x004c0000
#define  SCCCA_SS_REV_DELETECOL     0x004d0000
#define  SCCCA_SS_REV_NEWCELL       0x004e0000
#define  SCCCA_SS_REV_CLEARCELL     0x004f0000
#define  SCCCA_SS_REV_MODIFYCELL    0x00500000
#define  SCCCA_SS_REV_OLDCELLCONTENT 0x00510000
#define  SCCCA_SS_REV_NEWCELLCONTENT 0x00520000
#define  SCCCA_SS_REV_MOVECELLS     0x00530000
#define  SCCCA_SS_REV_OLDCELLLOCATION 0x00540000
#define  SCCCA_SS_REV_NEWCELLLOCATION 0x00550000
#define  SCCCA_SS_REV_ADDSHEET      0x00560000
#define  SCCCA_SS_REV_RENAMESHEET   0x00570000
#define  SCCCA_SS_REV_FORMAT        0x00580000
#define  SCCCA_SS_USERNAME          0x00590000
#define  SCCCA_SS_SHEETNAMES        0x005a0000
#define  SCCCA_SS_SHEETNAME         0x005b0000
#define  SCCCA_SHAPEIDENTIFIER      0x005c0000

#define  SCCCA_ENDSECTION_TAGSUBTYPE

/* If dwType equals SCCCA_BEGINTAG and dwSubType equals SCCCA_STYLE, then dwData1 can be */
#define SCCCA_STYLEFLAG_NONE		0x00000000
#define SCCCA_STYLEFLAG_INLINE_NUMBERING	0x00000001

/* If dwType equals SCCCA_FILEPROPERTY then dwSubType can ... */
#define  SCCCA_BEGINSECTION_FILEPROPERTYSUBTYPE "FilepropertySubtype"

#define  SCCCA_FILEID               0x0001

#define  SCCCA_ENDSECTION_FILEPROPERTYSUBTYPE



/* Possible values for dwData1 element of SCCCAGETCONTENT */
/*
|   If dwType equals SCCCA_TEXT
|   then dwData1 is the number of characters represented by the data
|   returned in pDataBuf (this may be different than the size of the
|   data returned in pDataBuf since DBCS and UNICODE output are not
|   one for one with the character count).
*/

/*
|   If dwType equals SCCCA_BEGINTAG or SCCCA_ENDTAG
|   and dwSubType equals SCCCA_DOCUMENTPROPERTY
|   then dwData1 can be:
*/
/* Document properties */
#define SCCCA_BEGINSECTION_TAGPROPERTYDATA1 "TagPropertyData1"

#define SCCCA_DOCCOMMENT            1
#define SCCCA_KEYWORD               2
#define SCCCA_LASTSAVEDBY           3
#define SCCCA_PRIMARYAUTHOR         4
#define SCCCA_SUBJECT               5
#define SCCCA_TITLE                 6
#define SCCCA_ABSTRACT              7
#define SCCCA_ACCOUNT               8
#define SCCCA_ADDRESS               9
#define SCCCA_ATTACHMENTS           10
#define SCCCA_AUTHORIZATION         11
#define SCCCA_BACKUPDATE            12
#define SCCCA_BILLTO                13
#define SCCCA_BLINDCOPY             14
#define SCCCA_CARBONCOPY            15
#define SCCCA_CATEGORY              16
#define SCCCA_CHECKEDBY             17
#define SCCCA_CLIENT                18
#define SCCCA_COMPLETEDDATE         19
#define SCCCA_COUNTCHARS            20
#define SCCCA_COUNTPAGES            21
#define SCCCA_COUNTWORDS            22
#define SCCCA_CREATIONDATE          23
#define SCCCA_DEPARTMENT            24
#define SCCCA_DESTINATION           25
#define SCCCA_DISPOSITION           26
#define SCCCA_DIVISION              27
#define SCCCA_EDITMINUTES           28
#define SCCCA_EDITOR                29
#define SCCCA_FORWARDTO             30
#define SCCCA_GROUP                 31
#define SCCCA_LANGUAGE              32
#define SCCCA_LASTPRINTDATE         33
#define SCCCA_MAILSTOP              34
#define SCCCA_MATTER                35
#define SCCCA_OFFICE                36
#define SCCCA_OPERATOR              37
#define SCCCA_OWNER                 38
#define SCCCA_PROJECT               39
#define SCCCA_PUBLISHER             40
#define SCCCA_PURPOSE               41
#define SCCCA_RECEIVEDFROM          42
#define SCCCA_RECORDEDBY            43
#define SCCCA_RECORDEDDATE          44
#define SCCCA_REFERENCE             45
#define SCCCA_REVISIONDATE          46
#define SCCCA_REVISIONNOTES         47
#define SCCCA_REVISIONNUMBER        48
#define SCCCA_SECONDARYAUTHOR       49
#define SCCCA_SECTION               50
#define SCCCA_SECURITY              51
#define SCCCA_SOURCE                52
#define SCCCA_STATUS                53
#define SCCCA_DOCTYPE               54
#define SCCCA_TYPIST                55
#define SCCCA_VERSIONDATE           56
#define SCCCA_VERSIONNOTES          57
#define SCCCA_VERSIONNUMBER         58
#define SCCCA_BASEFILELOCATION      59
#define SCCCA_MANAGER               60
#define SCCCA_COMPANY               61
#define SCCCA_LASTSAVEDATE          62
#define SCCCA_PRESENTATIONFORMAT    63
#define SCCCA_COUNTBYTES            64
#define SCCCA_COUNTLINES            65
#define SCCCA_COUNTPARAS            66
#define SCCCA_COUNTSLIDES           67
#define SCCCA_COUNTSLIDESHIDDEN     68
#define SCCCA_COUNTNOTES            69
#define SCCCA_COUNTMMCLIPS          70
#define SCCCA_COUNTCHARSWITHSPACES  71
#define SCCCA_SCALECROP             72
#define SCCCA_LINKSDIRTY            73
#define SCCCA_TITLEOFPARTS          74
#define SCCCA_HEADINGPAIRS          75
#define SCCCA_DOCNUMBER             76
/* ID3 Document Properties          */
#define SCCCA_ID3_AENC              77
#define SCCCA_ID3_APIC              78
#define SCCCA_ID3_ASPI              79
#define SCCCA_ID3_COMM              80
#define SCCCA_ID3_COMR              81
#define SCCCA_ID3_ENCR              82
#define SCCCA_ID3_EQUA              83
#define SCCCA_ID3_EQU2              84
#define SCCCA_ID3_ETCO              85
#define SCCCA_ID3_GEOB              86
#define SCCCA_ID3_GRID              87
#define SCCCA_ID3_IPLS              88
#define SCCCA_ID3_LINK              89
#define SCCCA_ID3_MCDI              90
#define SCCCA_ID3_MLLT              91
#define SCCCA_ID3_OWNE              92
#define SCCCA_ID3_PRIV              93
#define SCCCA_ID3_PCNT              94
#define SCCCA_ID3_POPM              95
#define SCCCA_ID3_POSS              96
#define SCCCA_ID3_RBUF              97
#define SCCCA_ID3_RVAD              98
#define SCCCA_ID3_RVA2              99
#define SCCCA_ID3_RVRB              100
#define SCCCA_ID3_SEEK              101
#define SCCCA_ID3_SIGN              102
#define SCCCA_ID3_SYLT              103
#define SCCCA_ID3_SYTC              104
#define SCCCA_ID3_TALB              105
#define SCCCA_ID3_TBPM              106
#define SCCCA_ID3_TCOM              107
#define SCCCA_ID3_TCON              108
#define SCCCA_ID3_TCOP              109
#define SCCCA_ID3_TDAT              110
#define SCCCA_ID3_TDLY              111
#define SCCCA_ID3_TENC              112
#define SCCCA_ID3_TEXT              113
#define SCCCA_ID3_TFLT              114
#define SCCCA_ID3_TIME              115
#define SCCCA_ID3_TIPL              116
#define SCCCA_ID3_TIT1              117
#define SCCCA_ID3_TIT2              118
#define SCCCA_ID3_TIT3              119
#define SCCCA_ID3_TKEY              120
#define SCCCA_ID3_TLAN              121
#define SCCCA_ID3_TLEN              122
#define SCCCA_ID3_TMCL              123
#define SCCCA_ID3_TMOO              124
#define SCCCA_ID3_TMED              125
#define SCCCA_ID3_TOAL              126
#define SCCCA_ID3_TOFN              127
#define SCCCA_ID3_TOLY              128
#define SCCCA_ID3_TOPE              129
#define SCCCA_ID3_TORY              130
#define SCCCA_ID3_TOWN              131
#define SCCCA_ID3_TPE1              132
#define SCCCA_ID3_TPE2              133
#define SCCCA_ID3_TPE3              134
#define SCCCA_ID3_TPE4              135
#define SCCCA_ID3_TPOS              136
#define SCCCA_ID3_TPRO              137
#define SCCCA_ID3_TPUB              138
#define SCCCA_ID3_TRCK              139
#define SCCCA_ID3_TRDA              140
#define SCCCA_ID3_TRSN              141
#define SCCCA_ID3_TRSO              142
#define SCCCA_ID3_TSIZ              143
#define SCCCA_ID3_TSRC              144
#define SCCCA_ID3_TSOA              145
#define SCCCA_ID3_TSOP              146
#define SCCCA_ID3_TSOT              147
#define SCCCA_ID3_TSSE              148
#define SCCCA_ID3_TSST              149
#define SCCCA_ID3_TYER              150
#define SCCCA_ID3_UFID              151
#define SCCCA_ID3_USER              152
#define SCCCA_ID3_USLT              153
#define SCCCA_ID3_WCOM              154
#define SCCCA_ID3_WCOP              155
#define SCCCA_ID3_WOAF              156
#define SCCCA_ID3_WOAR              157
#define SCCCA_ID3_WOAS              158
#define SCCCA_ID3_WORS              159
#define SCCCA_ID3_WPAY              160
#define SCCCA_ID3_WPUB              161
#define SCCCA_APPVERSION            162
/* Multimedia document properties */
#define SCCCA_MULTIMEDIA_ALBUM                163
#define SCCCA_MULTIMEDIA_ARRANGER             164
#define SCCCA_MULTIMEDIA_ARTIST               165
#define SCCCA_MULTIMEDIA_AUTHOR               166
#define SCCCA_MULTIMEDIA_CHAPTER              167
#define SCCCA_MULTIMEDIA_COMMENT              168
#define SCCCA_MULTIMEDIA_COMPOSER             169
#define SCCCA_MULTIMEDIA_COPYRIGHT            170
#define SCCCA_MULTIMEDIA_CREATIONDATE         171
#define SCCCA_MULTIMEDIA_DESCRIPTION          172
#define SCCCA_MULTIMEDIA_DIRECTOR             173
#define SCCCA_MULTIMEDIA_DISCLAIMER           174
#define SCCCA_MULTIMEDIA_EDITDATEDESCRIPTION  175
#define SCCCA_MULTIMEDIA_ENCODER              176
#define SCCCA_MULTIMEDIA_FORMAT               177
#define SCCCA_MULTIMEDIA_GENRE                178
#define SCCCA_MULTIMEDIA_HOSTCOMPUTER         179
#define SCCCA_MULTIMEDIA_INFO                 180
#define SCCCA_MULTIMEDIA_ISRC                 181
#define SCCCA_MULTIMEDIA_RECORDLABEL          182
#define SCCCA_MULTIMEDIA_RECORDLABELURL       183
#define SCCCA_MULTIMEDIA_FILECREATOR          184
#define SCCCA_MULTIMEDIA_FILECREATORURL       185
#define SCCCA_MULTIMEDIA_MODEL                186
#define SCCCA_MULTIMEDIA_TITLE                187
#define SCCCA_MULTIMEDIA_ORIGINALARTIST       188
#define SCCCA_MULTIMEDIA_RECORDINGCOPYRIGHT   189
#define SCCCA_MULTIMEDIA_PRODUCER             190
#define SCCCA_MULTIMEDIA_PERFORMER            191
#define SCCCA_MULTIMEDIA_PERFORMERURL         192
#define SCCCA_MULTIMEDIA_PRODUCT              193
#define SCCCA_MULTIMEDIA_HWSWREQS             194
#define SCCCA_MULTIMEDIA_SUBTITLE             195
#define SCCCA_MULTIMEDIA_SOURCECREDITS        196
#define SCCCA_MULTIMEDIA_SONGWRITER           197
#define SCCCA_MULTIMEDIA_SOFTWARE             198
#define SCCCA_MULTIMEDIA_TRACK                199
#define SCCCA_MULTIMEDIA_WARNING              200
#define SCCCA_MULTIMEDIA_WRITER               201
#define SCCCA_MULTIMEDIA_URL                  202
#define SCCCA_MULTIMEDIA_WIDTH                203
#define SCCCA_MULTIMEDIA_HEIGHT               204
#define SCCCA_MULTIMEDIA_RATING               205
#define SCCCA_MULTIMEDIA_ENCODERDESCRIPTION   206
#define SCCCA_MULTIMEDIA_KEYWORDS             207
#define SCCCA_MULTIMEDIA_RELATED              208
#define SCCCA_MULTIMEDIA_CREATIONTIME         209
#define SCCCA_MULTIMEDIA_MANUFACTURER         210
#define SCCCA_MULTIMEDIA_ARCHIVE              211
#define SCCCA_MULTIMEDIA_COMMISSIONEDBY       212
#define SCCCA_MULTIMEDIA_CROPPED              213
#define SCCCA_MULTIMEDIA_DIMENSIONS           214
#define SCCCA_MULTIMEDIA_RESOLUTION           215
#define SCCCA_MULTIMEDIA_ENGINEER             216
#define SCCCA_MULTIMEDIA_LIGHTNESS            217
#define SCCCA_MULTIMEDIA_MEDIUM               218
#define SCCCA_MULTIMEDIA_PALETTE              219
#define SCCCA_MULTIMEDIA_SUBJECT              220
#define SCCCA_MULTIMEDIA_SHARPNESS            221
#define SCCCA_MULTIMEDIA_SOURCEFORM           222
#define SCCCA_MULTIMEDIA_TECHNICIAN           223
#define SCCCA_MULTIMEDIA_NUMBEROFCHANNELS     224
#define SCCCA_MULTIMEDIA_SAMPLINGRATE         225
#define SCCCA_MULTIMEDIA_GROUPING             226
#define SCCCA_MULTIMEDIA_CATEGORY             227
#define SCCCA_MULTIMEDIA_LYRICS               228
#define SCCCA_MULTIMEDIA_TVNETWORK            229
#define SCCCA_MULTIMEDIA_TVSHOW               230
#define SCCCA_MULTIMEDIA_EPISODE              231
#define SCCCA_MULTIMEDIA_PURCHASEDATE         232

#define SCCCA_XMPNAMESPACE                    233
#define SCCCA_DOCTEMPLATE                     234

/* Image document properties */
#define SCCCA_IMAGE_IMAGEWIDTH                235
#define SCCCA_IMAGE_IMAGEHEIGHT               236
#define SCCCA_IMAGE_BITDEPTH                  237

/* system metadata as the document property */
#define  SCCCA_SYSTEM_FILESIZE      250
#define  SCCCA_SYSTEM_FILECREATED   251
#define  SCCCA_SYSTEM_FILEMODIFIED  252

#define SCCCA_USERDEFINEDPROP       500

#define SCCCA_ENDSECTION_TAGPROPERTYDATA1



/*
|   If dwType equals SCCCA_BEGINTAG or SCCCA_ENDTAG
|   and dwSubType equals SCCCA_EMAILFIELD
|   then dwData1 can be:
*/
/*  Base must equal the first e-mail related field.  */
#define SCCCA_BEGINSECTION_TAGEMAILDATA1 "TagEmailData1"

#define SCCCA_MAIL_BASE                   0x0001366A

#define SCCCA_MAIL_TO                     0x0001366A
#define SCCCA_MAIL_CC                     0x0001366B
#define SCCCA_MAIL_BCC                    0x0001366C
#define SCCCA_MAIL_SUBJECT                0x0001366D
#define SCCCA_MAIL_MSGFLAG                0x0001366E
#define SCCCA_MAIL_FLAGSTS                0x0001366F
#define SCCCA_MAIL_EXPIRES                0x00013670
#define SCCCA_MAIL_CATEGORIES             0x00013671
#define SCCCA_MAIL_IMPORTANCE             0x00013672
#define SCCCA_MAIL_SENSITIVITY            0x00013673
#define SCCCA_MAIL_LOCATION               0x00013674
#define SCCCA_MAIL_FULLNAME               0x00013675
#define SCCCA_MAIL_JOBTITLE               0x00013676
#define SCCCA_MAIL_COMPANY                0x00013677
#define SCCCA_MAIL_EMAIL                  0x00013678
#define SCCCA_MAIL_WEBPAGE                0x00013679
#define SCCCA_MAIL_WORKPHONE              0x0001367A
#define SCCCA_MAIL_HOMEPHONE              0x0001367B
#define SCCCA_MAIL_FROM                   0x0001367C
#define SCCCA_MAIL_ATTACHMENT             0x0001367D
#define SCCCA_MAIL_RTFBODY                0x0001367E
#define SCCCA_MAIL_RECEIVED               0x0001367F
#define SCCCA_MAIL_SIZE                   0x00013680
#define SCCCA_MAIL_LASTMODIFIED           0x00013681
#define SCCCA_MAIL_NEWSGROUPS             0x00013682
#define SCCCA_MAIL_SUBMITTIME             0x00013683
#define SCCCA_MAIL_CCME                   0x00013684
/*These following fields were specifically added for extracting the fields from a PST message*/
#define SCCCA_MAIL_ALTERNATE_RECIPIENT_ALLOWED      0x00013685
#define SCCCA_MAIL_CLIENT_SUBMIT_TIME               0x00013686
#define SCCCA_MAIL_CREATION_TIME                    0x00013687
#define SCCCA_MAIL_CONVERSATION_INDEX               0x00013688
#define SCCCA_MAIL_CONVERSATION_TOPIC               0x00013689
#define SCCCA_MAIL_MESSAGE_SUBMISSION_ID            0x0001368A
#define SCCCA_MAIL_MESSAGE_CLASS                    0x0001368B
#define SCCCA_MAIL_ORIGINATOR_DELIVERY_REPORT_REQUESTED 0x0001368C
#define SCCCA_MAIL_READ_RECEIPT_REQUESTED           0x0001368D
#define SCCCA_MAIL_RCVD_REPRESENTING_ADDRTYPE       0x0001368E
#define SCCCA_MAIL_RCVD_REPRESENTING_EMAIL_ADDRESS  0x0001368F
#define SCCCA_MAIL_RCVD_REPRESENTING_ENTRYID        0x00013690
#define SCCCA_MAIL_RCVD_REPRESENTING_NAME           0x00013691
#define SCCCA_MAIL_RCVD_REPRESENTING_SEARCH_KEY     0x00013692
#define SCCCA_MAIL_RECEIVED_BY_ADDRTYPE             0x00013693
#define SCCCA_MAIL_RECEIVED_BY_EMAIL_ADDRESS        0x00013694
#define SCCCA_MAIL_RECEIVED_BY_ENTRYID              0x00013695
#define SCCCA_MAIL_RECEIVED_BY_NAME                 0x00013696
#define SCCCA_MAIL_RECEIVED_BY_SEARCH_KEY           0x00013697
#define SCCCA_MAIL_RTF_IN_SYNC                      0x00013698
#define SCCCA_MAIL_RTF_SYNC_BODY_COUNT              0x00013699
#define SCCCA_MAIL_RTF_SYNC_BODY_CRC                0x0001369A
#define SCCCA_MAIL_RTF_SYNC_BODY_TAG                0x0001369B
#define SCCCA_MAIL_RTF_SYNC_PREFIX_COUNT            0x0001369C
#define SCCCA_MAIL_RTF_SYNC_TRAILING_COUNT          0x0001369D
#define SCCCA_MAIL_SEARCH_KEY                       0x0001369E
#define SCCCA_MAIL_SENDER_ADDRTYPE                  0x0001369F
#define SCCCA_MAIL_SENDER_EMAIL_ADDRESS             0x000136A0
#define SCCCA_MAIL_SENDER_ENTRYID                   0x000136A1
#define SCCCA_MAIL_SENDER_NAME                      0x000136A2
#define SCCCA_MAIL_SENDER_SEARCH_KEY                0x000136A3
#define SCCCA_MAIL_SENT_REPRESENTING_ADDRTYPE       0x000136A4
#define SCCCA_MAIL_SENT_REPRESENTING_EMAIL_ADDRESS  0x000136A5
#define SCCCA_MAIL_SENT_REPRESENTING_ENTRYID        0x000136A6
#define SCCCA_MAIL_SENT_REPRESENTING_SEARCH_KEY     0x000136A7
#define SCCCA_MAIL_TRANSPORT_MESSAGE_HEADERS        0x000136A8
#define SCCCA_MAIL_PRIORITY                         0x000136A9
#define SCCCA_MAIL_AUTO_FORWARDED                   0x000136AA
#define SCCCA_MAIL_DEFERRED_DELIVERY_TIME           0x000136AB
#define SCCCA_MAIL_EXPIRY_TIME                      0x000136AC
#define SCCCA_MAIL_LATEST_DELIVERY_TIME             0x000136AD
#define SCCCA_MAIL_RECIPIENT_REASSIGNMENT_PROHIBITED    0x000136AE
#define SCCCA_MAIL_REPLY_TIME                       0x000136AF
#define SCCCA_MAIL_REPORT_TAG                       0x000136B0
#define SCCCA_MAIL_RESPONSE_REQUESTED               0x000136B1
#define SCCCA_MAIL_REPLY_REQUESTED                  0x000136B2
#define SCCCA_MAIL_DELETE_AFTER_SUBMIT              0x000136B3
#define SCCCA_MAIL_MESSAGE_LOCALE_ID                0x000136B4
#define SCCCA_MAIL_CREATOR_NAME                     0x000136B5
#define SCCCA_MAIL_CREATOR_ENTRYID                  0x000136B6
#define SCCCA_MAIL_LAST_MODIFIER_NAME               0x000136B7
#define SCCCA_MAIL_LAST_MODIFIER_ENTRYID            0x000136B8
#define SCCCA_MAIL_INTERNET_ARTICLE_NUMBER          0x000136B9
#define SCCCA_MAIL_NT_SECURITY_DESCRIPTOR           0x000136BA
#define SCCCA_MAIL_TRUST_SENDER                     0x000136BB
#define SCCCA_MAIL_INTERNET_MESSAGE_ID              0x000136BC
#define SCCCA_MAIL_ATTR_HIDDEN                      0x000136BD
#define SCCCA_MAIL_ATTR_SYSTEM                      0x000136BE
#define SCCCA_MAIL_ATTR_READONLY                    0x000136BF
#define SCCCA_MAIL_INTERNET_CPID                    0x000136C0
#define SCCCA_MAIL_MESSAGE_CODEPAGE                 0x000136C1
#define SCCCA_MAIL_SENDER_FLAGS                     0x000136C2
#define SCCCA_MAIL_SENT_REPRESENTING_FLAGS          0x000136C3
#define SCCCA_MAIL_RCVD_BY_FLAGS                    0x000136C4
#define SCCCA_MAIL_RCVD_REPRESENTING_FLAGS          0x000136C5
#define SCCCA_MAIL_INET_MAIL_OVERRIDE_FORMAT        0x000136C6
#define SCCCA_MAIL_MSG_EDITOR_FORMAT                0x000136C7
#define SCCCA_MAIL_PROFILE_CONNECT_FLAGS            0x000136C8
#define SCCCA_MAIL_SENT_REPRESENTING_NAME           0x000136C9
#define SCCCA_MAIL_ENTRYID                          0x000136CA
#define SCCCA_MAIL_NORMALIZED_SUBJECT               0x000136CB
#define	SCCCA_MAIL_ATTENDEES						0x000136CC
#define	SCCCA_MAIL_REQATTENDEE						0x000136CD
#define	SCCCA_MAIL_OPTATTENDEE						0x000136CE			
#define	SCCCA_MAIL_FILEAS							0x000136CF
#define	SCCCA_MAIL_DISPLAYAS						0x000136D0
#define	SCCCA_MAIL_TITLE							0x000136D1
#define	SCCCA_MAIL_SUFFIX							0x000136D2
#define	SCCCA_MAIL_NICKNAME							0x000136D3
#define	SCCCA_MAIL_PROFESSION						0x000136D4
#define	SCCCA_MAIL_DEPARTMENT						0x000136D5
#define	SCCCA_MAIL_OFFICE							0x000136D6
#define	SCCCA_MAIL_ANNIVERSARY						0x000136D7
#define	SCCCA_MAIL_BIRTHDATE						0x000136D8
#define	SCCCA_MAIL_ASSISTANTSNAME					0x000136D9
#define	SCCCA_MAIL_SPOUSESNAME						0x000136DA
#define	SCCCA_MAIL_MANAGERSNAME						0x000136DB
#define	SCCCA_MAIL_BUSINESSADDRESS					0x000136DC
#define	SCCCA_MAIL_BUSINESSPHONE					0x000136DD
#define	SCCCA_MAIL_HOMEADDRESS						0x000136DE
#define	SCCCA_MAIL_OTHERADDRESS						0x000136DF
#define	SCCCA_MAIL_MOBILEPHONE						0x000136E0
#define	SCCCA_MAIL_BUSINESSFAX						0x000136E1
#define	SCCCA_MAIL_IMADDRESS						0x000136E2
#define	SCCCA_MAIL_INTERNETFREEBUSYADDR				0x000136E3
#define	SCCCA_MAIL_REMINDERTOPIC					0x000136E4
#define	SCCCA_MAIL_CONTACTS							0x000136E5
#define	SCCCA_MAIL_CALLBACKPHONE					0x000136E6
#define	SCCCA_MAIL_FIRST							0x000136E7
#define	SCCCA_MAIL_FAMILY							0x000136E8
#define	SCCCA_MAIL_TELENUMBER						0x000136E9
#define	SCCCA_MAIL_BUSINESSPHONE2					0x000136EA
#define	SCCCA_MAIL_RADIOPHONE						0x000136EB
#define	SCCCA_MAIL_CARPHONE							0x000136EC
#define	SCCCA_MAIL_OTHERPHONE						0x000136ED
#define	SCCCA_MAIL_PAGERPHONE						0x000136EE
#define	SCCCA_MAIL_OTHERFAX							0x000136EF
#define	SCCCA_MAIL_HOMEFAX							0x000136F0
#define	SCCCA_MAIL_TELEXPHONE						0x000136F1
#define	SCCCA_MAIL_ISDN								0x000136F2
#define	SCCCA_MAIL_ASSISTANTPHONE					0x000136F3
#define	SCCCA_MAIL_HOMEPHONE2						0x000136F4
#define	SCCCA_MAIL_MIDDLE							0x000136F5
#define	SCCCA_MAIL_TTYTTDPHONE						0x000136F6
#define	SCCCA_MAIL_GENDER							0x000136F7
#define	SCCCA_MAIL_PERSONALHOMEPAGE					0x000136F8
#define	SCCCA_MAIL_COMPANYPHONE						0x000136F9
#define	SCCCA_MAIL_HOMECITY							0x000136FA
#define	SCCCA_MAIL_HOMECOUNTRY						0x000136FB
#define	SCCCA_MAIL_POSTALCODE						0x000136FC
#define	SCCCA_MAIL_HOMESTATE						0x000136FD
#define	SCCCA_MAIL_HOMESTREET						0x000136FE
#define	SCCCA_MAIL_HOMEPOBOX						0x000136FF
#define	SCCCA_MAIL_OTHERCITY						0x00013700
#define	SCCCA_MAIL_OTHERCOUNTRY						0x00013701
#define	SCCCA_MAIL_OTHERPOSTALCODE					0x00013702
#define	SCCCA_MAIL_OTHERSTATE						0x00013703
#define	SCCCA_MAIL_OTHERSTREET						0x00013704
#define	SCCCA_MAIL_OTHERPOBOX						0x00013705
#define	SCCCA_MAIL_BUSINESSSTREET					0x00013706
#define	SCCCA_MAIL_BUSINESSCITY						0x00013707
#define	SCCCA_MAIL_BUSINESSSTATE					0x00013708
#define	SCCCA_MAIL_BUSINESSPOSTALCODE				0x00013709
#define	SCCCA_MAIL_BUSINESSCOUNTRY					0x0001370A
#define	SCCCA_MAIL_BUSINESSPOBOX					0x0001370B
#define	SCCCA_MAIL_DISPLAYAS2						0x0001370C
#define	SCCCA_MAIL_EMAIL2							0x0001370D
#define	SCCCA_MAIL_DISPLAYAS3						0x0001370E
#define	SCCCA_MAIL_EMAIL3							0x0001370F
#define	SCCCA_MAIL_DTSTART							0x00013710
#define	SCCCA_MAIL_DTEND							0x00013711
#define	SCCCA_MAIL_START							0x00013712
#define	SCCCA_MAIL_DURATION							0x00013713
#define	SCCCA_MAIL_ACCUREATTYPE1					0x00013714
#define	SCCCA_MAIL_ENTRY_TYPE						0x00013715
#define	SCCCA_MAIL_STATUS							0x00013716
#define	SCCCA_MAIL_PERCENT_COMPLETE					0x00013717
#define	SCCCA_MAIL_START_DATE						0x00013718
#define	SCCCA_MAIL_DUE_DATE							0x00013719
#define	SCCCA_MAIL_COMPLETED						0x0001371A
#define	SCCCA_MAIL_ACTUALWORK						0x0001371B
#define	SCCCA_MAIL_TOTAL_WORK						0x0001371C
#define	SCCCA_MAIL_OWNER							0x0001371D
#define	SCCCA_MAIL_BILLING							0x0001371E
#define	SCCCA_MAIL_MILEAGE							0x0001371F
#define SCCCA_MAIL_PROOF_OF_DELIVERY_REQUESTED		0x00013720
#define SCCCA_MAIL_PROOF_OF_SUBMISSION_REQUESTED    0x00013721
#define SCCCA_MAIL_IN_REPLY_TO_ID                   0x00013722
#define SCCCA_MAIL_TASK_COMPLETE					0x00013723
#define SCCCA_MAIL_APPOINTMENT_DURATION				0x00013724
#define SCCCA_MAIL_RECURRENCE_TYPE                  0x00013725
#define SCCCA_MAIL_RECURRENCE_RANGE                 0x00013726
#define SCCCA_MAIL_ORGANIZER                        0x00013727
#define SCCCA_MAIL_CALENDAR_EXCEPTION               0x00013728
/*  Default used if we can not retrieve the e-mail field id for any reason.  */
#define SCCCA_MAIL_UNKNOWN                0x00013729
/*  End must equal the last e-mail related field. WARNING!!!!
    There are two constants in ut/sccopp.h that are defined
    to be the same value as SCCCA_MAIL_BASE and SCCCA_MAIL_END. If you change
    either of these values, you must change the corresponding SCCOPT_MAIL_BASE
    or SCCOPT_MAIL_END values as well.
*/
#define SCCCA_MAIL_END                    0x00013729

#define SCCCA_MAIL_USERDEFINED            0x00014000

#define SCCCA_ENDSECTION_TAGEMAILDATA1



/*
|   If dwType equals SCCCA_BEGINTAG and dwSubType equals SCCCA_DOCUMENTPROPERTY
|   then dwData1 can be:
*/
#define SCCCA_BEGINSECTION_TAGEMAILTYPE "TagEmailType"

#define SCCCA_MAILTYPE_EMAIL        0
#define SCCCA_MAILTYPE_JOURNAL      1
#define SCCCA_MAILTYPE_CONTACT      2
#define SCCCA_MAILTYPE_NOTE         3
#define SCCCA_MAILTYPE_APPOINTMENT  4
#define SCCCA_MAILTYPE_TASK         5

#define SCCCA_ENDSECTION_TAGEMAILTYPE




/* values for SCCCA_SUBDOCPROPERTY */
#define  SCCCA_BEGINSECTION_TAGSUBDOCPROPDATA1 "TagSubdocpropData1"

#define  SCCCA_SUBDOC_AUTHOR               1
#define  SCCCA_SUBDOC_CREATEDATE           2
#define  SCCCA_SUBDOC_LASTSAVEDATE         3
#define  SCCCA_SUBDOC_TITLE                4
#define  SCCCA_SUBDOC_NOTES                5
#define  SCCCA_SUBDOC_AUTHORSHORT          6   /* either initials or short version of name */

#define  SCCCA_ENDSECTION_TAGSUBDOCPROPDATA1
                    

/* values for SCCCA_COMMENTREFERENCE */
#define SCCCA_BEGINSECTION_COMMENTREFDATA1 "CommentrefData1"

#define SCCCA_COMMENT_PARAGRAPH    0
#define SCCCA_COMMENT_CELL         1
#define SCCCA_COMMENT_SLIDE        2
#define SCCCA_COMMENT_VECTORPAGE   3

#define SCCCA_ENDSECTION_COMMENTREFDATA1


/*
|   If dwType equals SCCCA_ENDTAG
|   and dwSubType equals SCCCA_DOCUMENTPROPERTY
|   then dwData1 can be:
*/
#define SCCCA_BEGINSECTION_ENDTAGPROPERTYDATA1 "EndtagPropertyData1"

#define SCCCA_NOGENERATEDTEXT       0
#define SCCCA_GENERATEDTEXT         1
#define SCCCA_COULDNOTGENERATE      0xffffffff

#define SCCCA_ENDSECTION_ENDTAGPROPERTYDATA1


/*
|   If dwType equals SCCCA_FILEPROPERTY
|   and dwSubType equals SCCCA_FILEID
|   then dwData1 can be one of the file id values found in sccfi.h
*/


/* Possible values for dwData2 element of SCCCAGETCONTENT */
/*
|   If dwType equals SCCCA_TEXT
|   then dwData2 is the character set of the data provided in pDataBuf.
*/

/* dwData3 and dwData4 are reserved */

/* Possible values of SCCCAGETCONTENT.dwSubType: */
/* If dwType equals SCCCA_STYLECHANGE then dwSubType can be ... */
#define  SCCCA_BEGINSECTION_STYLECHANGESUBTYPE "StylechangeSubtype"

#define  SCCCA_PARASTYLE         0x00000001
#define  SCCCA_CHARSTYLE         0x00000002
/*
    Height and spacing are specified together because spacing can be relative
    to height.
*/
#define SCCCA_HEIGHTANDSPACING  0x00000003
/*
    Indents are specified together because they can all be specified at once and
    because first line indent can be relative to left indent.
*/
#define SCCCA_INDENTS               0x00000004
#define SCCCA_OCE                   0x00000005  /* Original character encoding.  */
#define SCCCA_TEXTCONT              0x00000006  /* Continuation for text.  */
#define SCCCA_GENERATEDTEXTCONT     0x00000007  /* Continuation for generated text.  */
#define SCCCA_OBJECTNAMECONT        0x00000008  /* Continuation for Object Name. */
#define SCCCA_OBJECTALTSTRINGCONT   0x00000009  /* Continuation for object Alt String*/

#define SCCCA_ENDSECTION_STYLECHANGESUBTYPE


/*
    Possible values of dwData1 when dwType = SCCCA_STYLECHANGE and
    dwSubType = SCCCA_PARASTYLE.
*/
#define SCCCA_BEGINSECTION_STYLECHANGEPARASTYLEDATA1 "StylechangeParastyleData1"

#define SCCCA_PARASTYLENAME     0x00000001

#define SCCCA_ENDSECTION_STYLECHANGEPARASTYLEDATA1


/*
    Possible values of dwData1 when dwType = SCCCA_STYLECHANGE and
    dwSubType = SCCCA_HEIGHTANDSPACING.  One or more orred together.
*/
#define SCCCA_BEGINSECTION_STYLECHANGEHEIGHTSPACINGDATA1 "StylechangeHeightspacingData1"

#define SCCCA_HEIGHT            0x00010000
#define SCCCA_SPACING           0x00020000

#define SCCCA_ENDSECTION_STYLECHANGEHEIGHTSPACINGDATA1


/*
    Possible values of dwData1 when dwType = SCCCA_STYLECHANGE and
    dwSubType = SCCCA_INDENTS.  One or more orred together.
*/
#define SCCCA_BEGINSECTION_STYLECHANGEINDENTDATA1 "StylechangeIndentData1"

#define SCCCA_LEFTINDENT        0x00010000
#define SCCCA_RIGHTINDENT       0x00020000
#define SCCCA_FIRSTINDENT       0x00040000

#define SCCCA_ENDSECTION_STYLECHANGEINDENTDATA1


/*  Possible values of dwSubType when dwType equals SCCCA_ANNOTATION.  */
#define SCCCA_BEGINSECTION_ANNOTATIONSUBTYPE "AnnotationSubtype"

#define SCCCA_ANNOTATION_FOOTNOTE   1
#define SCCCA_ANNOTATION_ENDNOTE    2
#define SCCCA_ANNOTATION_ANNOTATION 3
#define SCCCA_ANNOTATION_NOTE       4
#define SCCCA_ANNOTATION_COMMENT    5
#define SCCCA_ANNOTATION_SLIDENOTE  6

#define SCCCA_ENDSECTION_ANNOTATIONSUBTYPE


/*
    CAContentStatus() values and types
    These values are consistent with EXExportStatus()
*/
typedef EXSTATUSINFORMATION SCCCASTATUSINFORMATION;
#define SCCCA_STATUS_INFORMATION EXSTATUS_INFORMATION
#define SCCCA_STATUS_VERSION1 EXSTATUSVERSION1
#define SCCCA_STATUS_VERSION2 EXSTATUSVERSION2

#define SCCCA_STATUS_VERSION_LATEST EXSTATUSVERSION_LATEST


/*
|  P R O T O T Y P E S
*/

DA_ENTRYSC DAERR DA_ENTRYMOD CAOpenContent(VTHDOC hDoc, VTLPHCONTENT phContent );
DA_ENTRYSC DAERR DA_ENTRYMOD CACloseContent(VTHCONTENT hContent);
DA_ENTRYSC DAERR DA_ENTRYMOD CAReadFirst(VTHCONTENT hContent, PSCCCAGETCONTENT pGetContent);
DA_ENTRYSC DAERR DA_ENTRYMOD CAReadNext(VTHCONTENT hContent, PSCCCAGETCONTENT  pGetContent);
DA_ENTRYSC DAERR DA_ENTRYMOD CATell(VTHCONTENT hContent, PSCCDAPOS pPos );
DA_ENTRYSC DAERR DA_ENTRYMOD CASeek(VTHCONTENT hContent,PSCCDAPOS pPos );
DA_ENTRYSC DAERR DA_ENTRYMOD CAContentStatus(VTHCONTENT hContent, VTDWORD dwStatusType, VTLPVOID pStatus);

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCCA_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCCA_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCCA_H */

#endif /* SCCCA_H */
