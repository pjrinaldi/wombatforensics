/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved. */

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_LODLG_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */


#define BUTTON_CANCEL               101
#define BUTTON_HELP                 102
#define BUTTON_MORE_DISPLAY         103
#define BUTTON_CHANGEDISPLAY        104
#define BUTTON_CHANGEHEADER         105
#define BUTTON_REBUILD              106
#define BUTTON_WRITE                107
#define BUTTON_JOBPROPERTIES        108
#define BUTTON_MORE_CLIPBOARD       109
#define BUTTON_MORE_PRINT           110
#define BUTTON_CHANGEPRINT          111
#define BUTTON_CHANGECLIPBOARD      112
#define BUTTON_SKIP                 116
#define BUTTON_RENAME               117
#define BUTTON_ALL                  118

#define STATIC_PRINTING             201
#define STATIC_FILE                 202
#define STATIC_ON                   203
#define STATIC_DEVICE               204
#define STATIC_CONNECTED            205
#define STATIC_PORT                 206
#define STATIC_TOP                  209
#define STATIC_BOTTOM               210
#define STATIC_LEFT                 211
#define STATIC_RIGHT                212
#define STATIC_PAGELAYOUT           214
#define STATIC_SAMPLE               215
#define STATIC_HEADERFONT           222
#define STATIC_DISPLAYFONT          224
#define STATIC_DISPLAYFONTBOX       223
#define STATIC_HEADERFONTBOX        225
#define STATIC_JOBNAME              226
#define STATIC_JOBNAMEHELP          227
#define STATIC_VIEWASBOX            228
#define STATIC_TEXTTOFIND           229
#define STATIC_FILTERINFO           230
#define STATIC_RTFEXTRA             231
#define STATIC_PRINTFONT            232
#define STATIC_PRINTFONTBOX         233
#define STATIC_CLIPBOARDFONT        234
#define STATIC_CLIPBOARDFONTBOX     235
#define STATIC_CHOOSEDIRMSG         236
#define STATIC_STOREDDIRSBOX        237
#define STATIC_PROGRESSRECT         238
#define STATIC_PROGRESSTEXT         239
#define STATIC_ARCSORTORDER         240
#define STATIC_DOCMARGINSBOX        241
#define STATIC_ENCODINGBOX          242
#define STATIC_SSPRINTMODEBOX       243
#define STATIC_DBPRINTMODEBOX       244

#define STATIC_BADNAME_NAMEBOX      251
#define STATIC_BADNAME_NAMEEDIT     252
#define STATIC_PARENFILE            253
#define STATIC_WARNING              254

#define RADIO_TEXTANSI7             301
#define RADIO_TEXTANSI8             302
#define RADIO_TEXTDOS7              303
#define RADIO_TEXTDOS8              304
#define RADIO_TEXTUNICODE           305
#define RADIO_HEX                   306
#define RADIO_DONOTVIEW             307
#define RADIO_BMPORIGINALASPECT     308
#define RADIO_BMPSTRETCHTOMARGINS   309
#define RADIO_VECSTRETCHTOMARGINS   310
#define RADIO_VECORIGINALASPECT     311
#define RADIO_FORWARD               312
#define RADIO_BACKWARD              313
#define RADIO_SSCOPYASTABLE         314
#define RADIO_SSCOPYUSINGOTABS      315
#define RADIO_SSCOPYUSINGTABS       316
#define RADIO_DBCOPYASTABLE         317
#define RADIO_DBCOPYUSINGOTABS      318
#define RADIO_DBCOPYUSINGTABS       319
#define RADIO_SORTNONE              320
#define RADIO_SORTNAME              321
#define RADIO_SORTSIZE              322
#define RADIO_SORTDATE              323
#define RADIO_PRINTALL              324
#define RADIO_PRINTSELECT           325
#define RADIO_PRINTPAGES            326
#define RADIO_IGNOREDIR             327
#define RADIO_RESTOREDIR            328
#define RADIO_SORTASCENDING         329
#define RADIO_SORTDESCENDING        330
#define RADIO_TEXTSHIFTJIS          331
#define RADIO_TEXTBIG5              332
#define RADIO_TEXTHANGUL            333
#define RADIO_TEXTGB                334
#define RADIO_JAPANEUC              335
#define RADIO_CYRILLIC1251          336
#define RADIO_CYRILLICKOI8          337
#define RADIO_TEXTLATIN2            338
#define RADIO_TEXT                  339
#define RADIO_TEXTMAC               340
#define RADIO_SSPRINTNOSCALE        341
#define RADIO_SSPRINTFITTOPAGE      342
#define RADIO_SSPRINTFITTOHEIGHT    343
#define RADIO_SSPRINTFITTOWIDTH     344
#define RADIO_DBPRINTNOSCALE        345
#define RADIO_DBPRINTFITTOPAGE      346
#define RADIO_DBPRINTFITTOHEIGHT    347
#define RADIO_DBPRINTFITTOWIDTH     348
#define RADIO_SSPRINTSCALETO        349
#define RADIO_SSPRINTFITTO          350
#define RADIO_SSPRINTACROSS         351
#define RADIO_SSPRINTDOWN           352
#define RADIO_JAPANJIS              353
#define RADIO_CENTRALEU1250         354
#define RADIO_REPLACE               355
#define RADIO_SKIP                  356
#define RADIO_GEN                   357
#define RADIO_CREATE                358
#define RADIO_2DIGITYEARS           359
#define RADIO_4DIGITYEARS           360
#define RADIO_REGISTRYYEARS         361
#define RADIO_UTF8                  362
#define RADIO_BMPDOCSIZEASPECT      363
#define RADIO_VECDOCSIZEASPECT      364

#define BOX_PRINTHEADER             401
#define BOX_SSPRINTGRIDLINES        402
#define BOX_SSPRINTHEADINGS         403
#define BOX_DBPRINTGRIDLINES        404
#define BOX_DBPRINTHEADINGS         405
#define BOX_SSSHOWGRIDLINES         407
#define BOX_DBSHOWGRIDLINES         408
#define BOX_BMPPRINTBORDER          406
#define BOX_VECPRINTBORDER          412
#define BOX_MATCHCASE               413
#define BOX_CLIPTEXT                414
#define BOX_CLIPRICHTEXT            415
#define BOX_CLIPAMI2                416
#define BOX_CLIPAMI                 417
#define BOX_CLIPPROWRITEPLUS        418
#define BOX_CLIPWORDSTAR            419
#define BOX_CLIPLEGACY              420
#define BOX_CLIPBITMAP              421
#define BOX_CLIPDIB                 422
#define BOX_CLIPMETAFILE            423
#define BOX_CLIPPALETTE             424
#define BOX_DBFIELDNAMESTOCLIP      425
#define BOX_PRINTCOLLATE            426
#define BOX_USEDOCMARGINS           427
#define BOX_CLIPUNICODE				428
#define BOX_ANTIALIAS            429

#define EDIT_TOP                    501
#define EDIT_BOTTOM                 502
#define EDIT_LEFT                   503
#define EDIT_RIGHT                  504
#define EDIT_JOBNAME                505
#define EDIT_TEXTTOFIND             506
#define EDIT_PRINTCOPIES            507
#define EDIT_PRINTFIRSTPAGE         508
#define EDIT_PRINTLASTPAGE          509
#define EDIT_SSPRINTSCALEPERCENT    510
#define EDIT_SSPRINTSCALEWIDE       511
#define EDIT_SSPRINTSCALEHIGH       512
#define EDIT_BADNAMETEXT            513

#define MSCROLL_TOP                 601
#define MSCROLL_BOTTOM              602
#define MSCROLL_LEFT                603
#define MSCROLL_RIGHT               604
#define MSCROLL_PERCENT             605
#define MSCROLL_WIDE                606
#define MSCROLL_HIGH                607

#define LIST_FILTERS                700
#define LIST_AVAILABLEPRINTERS      701

#define SCCLO_PRINTABORTDIALOG       1000
#define SCCLO_CDPRINTDLG             1001
#define SCCLO_PRINTOPTIONSDIALOG     1002
#define SCCLO_DISPLAYOPTIONSDIALOG   1003
#define SCCLO_PRINTMOREDIALOG        1004
#define SCCLO_DISPLAYMOREDIALOG      1005
#define SCCLO_SEARCHDIALOG           1006
#define SCCLO_CLIPBOARDOPTIONSDIALOG 1007
#define SCCLO_CLIPBOARDMOREDIALOG    1008
#define SCCLO_FILTERLISTDIALOG       1009

#define SCCLO_PRINTOPTIONSPAGE       1010
#define SCCLO_CLIPBOARDOPTIONSPAGE   1011
#define SCCLO_DISPLAYOPTIONSPAGE     1012

#define SCCLO_ARCSAVEDLGTEMPLATE     1013
#define SCCLO_PROGRESSDIALOG         1014

#define SCCLO_CDPRINTDLG95           1015

#define SCCLO_ARCBADNAMEDIALOG       1023
#define SCCLO_ARCILLEGALCHARDIALOG   1024


#if (defined(WINDOWS) || defined(_WINDOWS))
#include <dlgs.h>    /* Common Dialog numbers */
#endif

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_LODLG_H
#pragma pack(pop)
#undef SCC_PACKED_BY_LODLG_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_LODLG_H */
