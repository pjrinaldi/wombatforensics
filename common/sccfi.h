/* Copyright (c) 2001, 2016, Oracle and/or its affiliates. 
All rights reserved. */

 /*
  |  Outside In Viewer Technology - Include
  |
  |  Include:      SCCFI.H
  |  Environment:  Portable
  |  Function:     Defines file type ids
  */

#ifndef SCCFI_H
#define SCCFI_H

#ifdef WINDOWS
#ifndef SCC_PACK_SET
#define SCC_PACK_SET 1
#define SCC_PACKED_BY_SCCFI_H 1
#pragma pack(push,8)
#endif /* SCC_PACK_SET */
#endif /* WINDOWS */

#if !defined(IMS_H) || !defined(IOS)
#ifndef RC_INVOKED  /* prevents the Windows Resource Compiler from wasting its time */
#include "scctype.h"
#include "sccio.h"
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

    /*
    |
    |   Format IDs
    |
    |   NOTE: Each section has a FI_xxxBEGIN and FI_xxxEND for use with Mac string selection.
    |         Be sure to update the FI_xxxEND when adding FI's
    */

#define FI_IDSTART              1000

    /*
    |   Word Processor
    */

#define FI_WPBEGIN              1000

#define FI_WORD4                1000
#define FI_WORD5                1001
#define FI_WORDSTAR5            1002
#define FI_WORDSTAR4            1003
#define FI_WORDSTAR2000         1004
#define FI_WORDPERFECT5         1005
#define FI_MULTIMATE36          1006
#define FI_MULTIMATEADV         1007
#define FI_RFT                  1008
#define FI_TXT                  1009    /* DisplayWrite 3 */
#define FI_SMART                1010
#define FI_SAMNA                1011
#define FI_PFSWRITEA            1012
#define FI_PFSWRITEB            1013
#define FI_PROWRITE1            1014
#define FI_PROWRITE2            1015
#define FI_IBMWRITING           1016
#define FI_FIRSTCHOICE          1017
#define FI_WORDMARC             1018
#define FI_DIF                  1019
#define FI_VOLKSWRITER          1020
#define FI_DX                   1021
#define FI_SPRINT               1022
#define FI_WORDPERFECT42        1023
#define FI_TOTALWORD            1024
#define FI_IWP                  1025
#define FI_WORDSTAR55           1026
#define FI_WANGWPS              1027
#define FI_RTF                  1028
#define FI_MACWORD3             1029
#define FI_MACWORD4             1030
#define FI_MASS11PC             1031
#define FI_MACWRITEII           1032
#define FI_XYWRITE              1033
#define FI_FFT                  1034
#define FI_MACWORDPERFECT       1035
#define FI_DISPLAYWRITE4        1036
#define FI_MASS11VAX            1037
#define FI_WORDPERFECT51        1038
#define FI_MULTIMATE40          1039
#define FI_QAWRITE              1040
#define FI_MULTIMATENOTE        1041
#define FI_PCFILELETTER         1043
#define FI_MANUSCRIPT1          1044
#define FI_MANUSCRIPT2          1045
#define FI_ENABLEWP             1046
#define FI_WINWRITE             1047
#define FI_WORKS1               1048
#define FI_WORKS2               1049
#define FI_WORDSTAR6            1050
#define FI_OFFICEWRITER         1051
#define FI_MACWORD4COMPLEX      1052
#define FI_DISPLAYWRITE5        1053
#define FI_WINWORD1             1054
#define FI_WINWORD1COMPLEX      1055
#define FI_AMI                  1056
#define FI_AMIPRO               1057
#define FI_FIRSTCHOICE3         1058
#define FI_MACWORDPERFECT2      1059
#define FI_MACWORKSWP2          1060
#define FI_PROWRITEPLUS         1061
#define FI_LEGACY               1062
#define FI_SIGNATURE            1063
#define FI_WINWORDSTAR          1064
#define FI_WINWORD2             1065
#define FI_JUSTWRITE            1066
#define FI_WORDSTAR7            1067
#define FI_WINWORKSWP           1068
#define FI_JUSTWRITE2           1069
#define FI_AMICLIP              1070
#define FI_LEGACYCLIP           1071
#define FI_PROWRITEPLUSCLIP     1072
#define FI_MACWORD5             1073
#define FI_ENABLEWP4            1074
#define FI_WORDPERFECT6         1075
#define FI_WORD6                1076
#define FI_DX31                 1077
#define FI_WPFENCRYPT           1078
#define FI_QAWRITE3             1079
#define FI_MACWORDPERFECT3      1080
#define FI_CEOWORD              1081
#define FI_WINWORD6             1082
#define FI_WORDPERFECT51J       1083
#define FI_ICHITARO3            1084
#define FI_ICHITARO4            1085
#define FI_WINWORD1J            1086
#define FI_WINWORD5J            1087
#define FI_MATSU4               1088
#define FI_MATSU5               1089
#define FI_P1                   1090
#define FI_RTFJ                 1091
#define FI_CEOWRITE             1092
#define FI_WINWORKSWP3          1093
#define FI_WORDPAD              1094
#define FI_WPFUNKNOWN           1095
#define FI_WINWORD2_OLECONV     1096
#define FI_WORDPERFECT61        1097
#define FI_FTDF                 1098
#define FI_WORDPERFECT5E        1099    /* Europa Fulcrum. */
#define FI_WORDPERFECT6E        1100    /* Europa Fulcrum. */
#define FI_HTML                 1101
#define FI_WINWORD7             1102
#define FI_AREHANGEUL           1103    /* No filter. Korean.*/
#define FI_HANA                 1104    /* No filter. Korean.*/
#define FI_WINWORKSWP4          1105
#define FI_PERFECTWORKS1        1106
#define FI_WORDPERFECT7         1107
#define FI_WORDPRO              1108
#define FI_HTML_LATIN2          1109
#define FI_HTML_JAPANESESJIS    1110
#define FI_HTML_JAPANESEEUC     1111
#define FI_HTML_CHINESEBIG5     1112
#define FI_HTML_CHINESEEUC      1113
#define FI_HTML_CHINESEGB       1114
#define FI_HTML_KOREANHANGUL    1115
#define FI_HTML_CYRILLIC1251    1116
#define FI_HTML_CYRILLICKOI8    1117
#define FI_CYRILLIC1251         1118
#define FI_CYRILLICKOI8         1119
#define FI_WWRITE_SHIFTJIS      1120
#define FI_WWRITE_CHINESEGB     1121
#define FI_WWRITE_HANGEUL       1122
#define FI_WWRITE_CHINESEBIG5   1123
#define FI_WPSPLUS              1124
#define FI_MACWORD6             1125
#define FI_WINWORD97            1126
#define FI_RAINBOW              1127
#define FI_INTERLEAF            1128
#define FI_MIFF3                1129
#define FI_MIFF4                1130
#define FI_MIFF5                1131
#define FI_TEXTMAIL             1132
#define FI_MACWORD97            1133
#define FI_INTERLEAFJ           1134
#define FI_MIFF3J               1135
#define FI_MIFF4J               1136
#define FI_MIFF5J               1137
#define FI_MIFF55               1138
#define FI_WORDPERFECT8         1139
#define FI_ICHITARO8            1140
#define FI_VCARD                1141
#define FI_HTML_CSS             1142
#define FI_OUTLOOK_MSG_MAIL     1143
#define FI_OUTLOOK_MSG          FI_OUTLOOK_MSG_MAIL  /* _MSG replaced by _MSG_SUBTYPE */
#define FI_POCKETWORD           1144
#define FI_WORDPRO97            1145
#define FI_WINWORD2000          1146    /* Word 2000                          */
#define FI_W2KHTML              1147    /* Word 2000 Save As... HTML          */
#define FI_XL2KHTML             1148    /* Excel 2000 Save As... HTML         */
#define FI_PP2KHTML             1149    /* PowerPoint 2000  Save As... HTML   */
#define FI_XML                  1150    /* XML                                */
#define FI_WML                  1151    /* WML [ASCII]                        */
#define FI_WMLB                 1152    /* WML [Binary]                       */
#define FI_HTML_JAPANESEJIS     1153
#define FI_WML_CHINESEBIG5      1154
#define FI_WML_CHINESEEUC       1155
#define FI_WML_CHINESEGB        1156
#define FI_WML_CYRILLIC1251     1157
#define FI_WML_CYRILLICKOI8     1158
#define FI_WML_JAPANESEJIS      1159
#define FI_WML_JAPANESESJIS     1160
#define FI_WML_JAPANESEEUC      1161
#define FI_WML_KOREANHANGUL     1162
#define FI_WML_LATIN2           1163
#define FI_WML_CSS              1164
#define FI_STAROFFICEWRITER52   1165    /* StarOffice Writer 5.2 */
#define FI_MIFF6                1166
#define FI_MIFF6J               1167
#define FI_MIFF                 1168
#define FI_JAVASCRIPT           1169
#define FI_TEXT                 1170    /*  Only used by Export - No filter associated with this FI ID */
#define FI_HDML                 1171
#define FI_CHTML                1172
#define FI_XHTMLB               1173
#define FI_HTMLAG               1174
#define FI_HTMLWCA              1175
/*  Searchml 2.0 deprecated.  */
/* #define FI_SEARCHML             1176 */
#define FI_POCKETWORD20         1177
#define FI_WIRELESSHTML         1178    /*  Only used by Export */
#define FI_HANGULWP97           1179
#define FI_HANGULWP2002         1180
#define FI_HTMLUNICODE          1181
#define FI_XML_DOCTYPE_HTML     1182
#define FI_PAGEML               1183
#define FI_EBCDIC               1184
#define FI_WINWORD2002          1185
#define FI_WINWORD2003          1186
#define FI_MIME                 1187
#define FI_STAROFFICEWRITER6    1188    /* StarOffice Writer 6 & 7 */
#define FI_OUTLOOK_PST          1189    /*PST 97/2000/XP file*/
#define FI_XHTML                1190    /* XML/WML/HTML test  */
#define FI_MSWORKS2000          1191    /* Microsoft Works 2000 */
#define FI_MIMEMAIL             1192
#define FI_MIMENEWS             1193
#define FI_MIMEOUTLOOKNEWS      1194
#define FI_MIMEOUTLOOKEML       1195
#define FI_VCAL                 1196    /*VCalendar*/
#define FI_TNEF                 1197
#define FI_MHTML                1198
#define FI_SEARCHHTML           1199    /*  Search Export HTML output.  Created by SearchML.  */

#define FI_WPEND                1199

    /*
    |   Database
    */

#define FI_DBBEGIN              1200

#define FI_SMARTDATA            1200
#define FI_DBASE3               1201
#define FI_DBASE4               1202
#define FI_FRAMEWORKIII         1203
#define FI_WORKSDATA            1204
#define FI_DATAEASE             1205
#define FI_PARADOX3             1206
#define FI_PARADOX35            1207
#define FI_QADBASE              1208
#define FI_REFLEX               1209
#define FI_RBASEV               1210
#define FI_RBASE5000            1211
#define FI_RBASEFILE1           1212
#define FI_RBASEFILE3           1213
#define FI_FIRSTCHOICE_DB       1214
#define FI_MACWORKSDB2          1215
#define FI_WINWORKSDB           1216
#define FI_PARADOX4             1217
#define FI_ACCESS1              1218
#define FI_CEODB                1219
#define FI_WINWORKSDB3          1220
#define FI_WINWORKSDB4          1221
#define FI_ACCESS7              1222
#define FI_MSPROJECT98          1223
#define FI_MSPROJECT2000        1224
#define FI_MSPROJECT2002        1225
#define FI_MSPROJECT2007        1226
#define FI_LOTUSNOTESDB         1227
#define FI_MSPROJECT2010        1228
#define FI_ACCESS2000           1229
#define FI_ACCESS2007           1230
#define FI_ACCESSWEBDATABASE    1231
#define FI_ACCESS2007TEMPLATES  1232
#define FI_MSPROJECT2013        1233
#define FI_DBEND                1233



#define FI_WPBEGIN2             1300

#define FI_SEARCHTEXT           1300    /*  Search Export text output.  */
#define FI_PSTF                 1301    /*  PST Fields File */
#define FI_PST_2003             1302    /*  PST 2003 File*/
#define FI_PAB_2002             1303    /*  Outlook PAB (Personal Address Book) file*/
/*  Searchml 2.0 deprecated.  */
/* #define FI_SEARCHML20           1304 */
#define FI_SEARCHML30           1305
#define FI_YAHOOIM              1306
#define FI_WORDXML2003          1307    /* MS Office 2003 Word XML format */
#define FI_WORDXML12            1308    /* MS Office 12 Word format */
#define FI_STAROFFICEWRITER8    1309    /* Star Office 8 & Open Office 2 */
#define FI_SEARCHML31           1310
#define FI_OUTLOOK_OFT_MAIL     1311    /* Microsoft Outlook Form Template.*/
#define FI_OUTLOOK_OFT          FI_OUTLOOK_OFT_MAIL   /* _OFT replaced by _OFT_SUBTYPE */ 
#define FI_WINWORD2007          1312    /* MS Office 12 (2007) Word XML format */
#define FI_ENCRYPTED_WORD2007   1313
#define FI_WINWORDTEMPLATE2007  1314    /* MS Office 12 (2007) Word Template XML format */
#define FI_SEARCHML32           1315
#define FI_DRM_UNKNOWN          1316
#define FI_DRM_WORD             1317
#define FI_DRM_WORD2007         1318
#define FI_ORACLE_IRM           1319
#define FI_XMP                  1320
#define FI_SEARCHML33           1321
#define FI_PHTML                1322
#define FI_OPENOFFICEWRITER1    1323    /* Open Office Writer 1 */
#define FI_OPENOFFICEWRITER2    1324    /* Open Office Writer 2 */
#define FI_SYMPHONYDOCUMENT     1325    /* IBM Lotus Symphony Document */
#define FI_SEARCHML34           1326
#define FI_WINWORD2007_MACRO    1327    /* MS Office 12 (2007) Word - Macro Enabled XML format*/
#define FI_WINWORDTEMPLATE2007_MACRO  1328    /* MS Office 12 (2007) Word Template - Macro Enabled XML format */
#define FI_MSWORDPICTURE        1329    /* Microsoft Word Picture */
#define FI_XMPS                 1330    /* Filter to generate XMP SO output */
#define FI_OPENOFFICEWRITER3    1331    /* Open Office Writer 3 */
#define FI_STAROFFICEWRITER9    1332
#define FI_ORACLEOPENOFFICEWRITER3 1333
#define FI_SAMSUNGJUNGUM        1334  /* Samsung Jungum .GUL File */
#define FI_KINGSOFTWRITER       1335
#define FI_WINWORD2010               1336
#define FI_WINWORDTEMPLATE2010       1337
#define FI_WINWORD2010_MACRO         1338
#define FI_WINWORDTEMPLATE2010_MACRO 1339
#define FI_SEARCHML_LATEST      1340    /*generic FI that will always point to newest version of SearchML.*/
#define FI_TRILLIAN_TEXT        1341
#define FI_TRILLIAN_XML         1342
#define FI_LIVEMESSENGER        1343
#define FI_AOLMESSENGER         1344

#define FI_OUTLOOK_MSG_APPT     1345 
#define FI_OUTLOOK_OFT_APPT     1346 
#define FI_OUTLOOK_MSG_JOURNAL  1347
#define FI_OUTLOOK_OFT_JOURNAL  1348
#define FI_OUTLOOK_MSG_CONTACT  1349
#define FI_OUTLOOK_OFT_CONTACT  1350
#define FI_OUTLOOK_MSG_NOTE     1351
#define FI_OUTLOOK_OFT_NOTE     1352
#define FI_OUTLOOK_MSG_TASK     1353
#define FI_OUTLOOK_OFT_TASK     1354

#define FI_ENCRYPTED_WORD2010   1355
#define FI_EMLX                 1356

#define FI_OUTLOOK_MSG_NDR      1357 /*Non delivery report*/
#define FI_OUTLOOK_OFT_NDR      1358
#define FI_OUTLOOK_MSG_POST     1359
#define FI_OUTLOOK_OFT_POST     1360
#define FI_OUTLOOK_MSG_DISTLIST 1361  /*Distribution list*/
#define FI_OUTLOOK_OFT_DISTLIST 1362
#define FI_OUTLOOK_MSG_CDSMAIL   1363 /*Clear digitally signed mail message*/
#define FI_OUTLOOK_OFT_CDSMAIL   1364
#define FI_OUTLOOK_MSG_ODSMAIL   1365 /*Opaque digitally signed mail message*/
#define FI_OUTLOOK_OFT_ODSMAIL   1366


#define FI_IWORK_PAGES          1367
#define FI_IWORK_PAGES_PREVIEW  1368
#define FI_SEARCHML35           1369
#define FI_SMIME                1370
#define FI_SMIME_CLEAR          1371

#define FI_WINWORD2013               1372
#define FI_WINWORDTEMPLATE2013       1373
#define FI_WINWORD2013_MACRO         1374
#define FI_WINWORDTEMPLATE2013_MACRO 1375

#define FI_HTML5                1376
#define FI_RAWTEXT              1377

#define FI_SEARCHML36           1378

#define FI_LIBREOFFICEWRITER3 1379
#define FI_LIBREOFFICEWRITER4 1380

#define FI_OLM2011              1381    /* Outlook for Mac 2011 archive */

#define FI_APACHEOFFICEWRITER3 1382
#define FI_APACHEOFFICEWRITER4 1383

#define FI_WINWORD2016               1384
#define FI_WINWORDTEMPLATE2016       1385
#define FI_WINWORD2016_MACRO         1386
#define FI_WINWORDTEMPLATE2016_MACRO 1387

#define FI_STRICTXML_WINWORD2013     1388
#define FI_STRICTXML_WINWORD2016     1389
#define FI_IWORK13_PAGES          1390
#define FI_WPEND2               1390


    /*
    |   Spreadsheet
    */

#define FI_SSBEGIN              1400

#define FI_SYMPHONY1            1400    /* Lotus Symphony 1.0                   */
#define FI_123R1                1401    /* Lotus 123 1.0 & 1.0A                 */
#define FI_123R2                1402    /* Lotus 123 2.0 & Symphony 1.1 & 2.0   */
#define FI_123R3                1403    /* Lotus 123 3.0                        */
#define FI_SMARTSHEET           1404
#define FI_EXCEL                1405
#define FI_ENABLESHEET          1406
#define FI_WORKSSHEET           1407
#define FI_VPPLANNER            1408
#define FI_TWIN                 1409
#define FI_SUPERCALC5           1410
#define FI_QUATTROPRO           1411
#define FI_QUATTRO              1412
#define FI_PFS_PLAN             1413
#define FI_FIRSTCHOICE_SS       1414
#define FI_EXCEL3               1415
#define FI_GENERIC_WKS          1416
#define FI_MACWORKSSS2          1417
#define FI_WINWORKSSS           1418
#define FI_EXCEL4               1419
#define FI_QUATTROPROWIN        1420
#define FI_123R4                1421    /* Lotus 123 4.0   Win                  */
#define FI_QUATTROPRO1J         1422    /* Quattro Japan                        */
#define FI_CEOSS                1423    /* CEO Spreadsheet                      */
#define FI_EXCEL5               1424
#define FI_MULTIPLAN4           1425
#define FI_WINWORKSSS3          1426
#define FI_QUATTROPRO4          1427
#define FI_QUATTROPRO5          1428
#define FI_QUATTROPRO6          1429
#define FI_123R2OS2             1430
#define FI_123R2OS2CHART        1431
#define FI_WINWORKSSS4          1432
#define FI_QUATTROPRO7NB        1433
#define FI_QUATTROPRO7GR        1434
#define FI_123R6                1435    /* Lotus    123 6.0   Win95 - Gemini    */
#define FI_MACEXCEL4            1436
#define FI_MACEXCEL5            1437
#define FI_EXCEL97              1438
#define FI_EXCEL3WORKBOOK       1439
#define FI_EXCEL4WORKBOOK       1440
#define FI_MACEXCEL4WORKBOOK    1441    /* with MacBinary header                */
#define FI_REGMACEXCEL4WB       1442    /* without MacBinary header             */
#define FI_123R9                1443    /* Lotus 123 - SmartSuite 98            */
#define FI_QUATTROPRO8          1444
#define FI_QUATTROPRO9NB        1445
#define FI_EXCEL2000            1446    /* Excel 2000                           */
#define FI_QUATTROPRO10NB       1447
#define FI_EXCEL2002            1448    /* Excel 2002                           */
#define FI_STAROFFICECALC52     1449    /* StarOffice Calc 5.2 */
#define FI_QUATTROPRO11NB       1450
#define FI_EXCEL2003            1451    /* Excel 2003                           */
#define FI_STAROFFICECALC6      1452    /* StarOffice Calc 6 & 7 */
#define FI_QUATTROPRO12NB       1453
#define FI_STAROFFICECALC8      1454    /* StarOffice Calc 8 & Open Office 2    */
#define FI_EXCEL2007            1455    /* Excel 2007                           */
#define FI_ENCRYPTED_EXCEL2007  1456
#define FI_EXCEL2007_BINARY     1457
#define FI_DRM_EXCEL            1458
#define FI_DRM_EXCEL2007        1459
#define FI_MSWORKSSS6           1460
#define FI_OPENOFFICECALC1      1461    /* Open Office Calc 6 */
#define FI_OPENOFFICECALC2      1462    /* Open Office Calc 8 */
#define FI_SYMPHONYSPREADSHEET  1463    /* IBM Lotus Symphony Spreadsheet */
#define FI_EXCELTEMPLATE2007    1464    /* Excel Template 2007                  */
#define FI_EXCEL2007_MACRO      1465    /* Excel Macro Enabled                     */
#define FI_EXCELTEMPLATE2007_MACRO    1466    /* Excel Template Macro Enabled 2007                  */
#define FI_EXCELXML2003         1467    /* Microsoft Office Excel 2002/2003 XML */
#define FI_OPENOFFICECALC3      1468    /* Open Office Calc 3 */
#define FI_QUATTROPROX4NB       1469
#define FI_ENCRYPTED_EXCEL2007_BINARY        1470
#define FI_EXCEL2007_ADDINMACRO 1471
#define FI_DIFSS                1472
#define FI_STAROFFICECALC9      1473
#define FI_ORACLEOPENOFFICECALC3 1474
#define FI_KINGSOFTSPREADSHEETS  1475
#define FI_EXCEL2010                 1476
#define FI_EXCEL2010_MACRO           1477
#define FI_EXCELTEMPLATE2010         1478 
#define FI_EXCELTEMPLATE2010_MACRO   1479
#define FI_EXCEL2010_ADDINMACRO      1480
#define FI_EXCEL2010_BINARY          1481
#define FI_ENCRYPTED_EXCEL2010        1482
#define FI_ENCRYPTED_EXCEL2010_BINARY 1483
#define FI_QUATTROPROX5NB            1484
#define FI_IWORK_NUMBERS             1485
#define FI_IWORK_NUMBERS_PREVIEW     1486
#define FI_EXCELXML2007              1487

#define FI_EXCEL2013                 1488
#define FI_EXCEL2013_MACRO           1489
#define FI_EXCELTEMPLATE2013         1490 
#define FI_EXCELTEMPLATE2013_MACRO   1491
#define FI_EXCEL2013_ADDINMACRO      1492
#define FI_EXCEL2013_BINARY          1493

#define FI_JSON_OISPREADSHEET        1494
#define FI_QUATTROPROX6NB            1495

#define FI_LIBREOFFICECALC3          1496
#define FI_LIBREOFFICECALC4          1497

#define FI_APACHEOFFICECALC3         1498
#define FI_APACHEOFFICECALC4         1499

#define FI_SSEND                     1499


    /*
    |   Graphic
    */

#define FI_GRAPHICBEGIN         1500

#define FI_BMP                  1500
#define FI_TIFF                 1501
#define FI_PCX                  1502
#define FI_GIF                  1503
#define FI_EPSTIFF              1504
#define FI_CCITTGRP3            1505
#define FI_MACPICT2             1506
#define FI_WPG                  1507
#define FI_WINDOWSMETA          1508
#define FI_LOTUSPIC             1509
#define FI_MACPICT1             1510
#define FI_AMIDRAW              1511
#define FI_TARGA                1512
#define FI_GEMIMG               1513
#define FI_OS2DIB               1514
#define FI_WINDOWSICON          1515
#define FI_WINDOWSCURSOR        1516
#define FI_MICROGRAFX           1517
#define FI_MACPAINT             1518
#define FI_CORELDRAW2           1520
#define FI_CORELDRAW3           1521
#define FI_HPGL                 1522
#define FI_HARVARDDOS3          1523
#define FI_HARVARDDOS2          1524
#define FI_HARVARDDOS3PRS       1525
#define FI_FREELANCE            1526
#define FI_WPG2                 1527
#define FI_CGM                  1528
#define FI_EXCELCHART           1529
#define FI_EXCEL3CHART          1530
#define FI_EXCEL4CHART          1531
#define FI_CANDY4               1532
#define FI_HANAKO1              1533
#define FI_HANAKO2              1534
#define FI_JPEGFIF              1535
#define FI_EXCEL5CHART          1536
#define FI_CORELDRAW4           1537
#define FI_POWERPOINT4          1538
#define FI_DCX                  1539
#define FI_POWERPOINT3          1540
#define FI_CORELDRAW5           1541
#define FI_OS2METAFILE          1542
#define FI_POWERPOINT7          1543
#define FI_DXFA                 1544
#define FI_DXFB                 1545
#define FI_DXB                  1546
#define FI_FREELANCE3           1547
#define FI_POWERPOINTMAC3       1548
#define FI_POWERPOINTMAC4       1549
#define FI_PRESENTATIONS        1550
#define FI_OS2WARPBMP           1551
#define FI_AUTOCADDWG12         1552
#define FI_AUTOCADDWG13         1553
#define FI_ILLUSTRATOR          1554
#define FI_PRESENTATIONS7       1555
#define FI_WPG7                 1556
#define FI_PDF                  1557
#define FI_FRAMEMAKER           1558
#define FI_SUNRASTER            1559
#define FI_AUTOSHADE            1560
#define FI_KODAKPCD             1561
#define FI_EXTPOWERPOINT4       1562
#define FI_EXTPOWERPOINTMAC4    1563
#define FI_ENHWINDOWSMETA       1564
#define FI_GEM                  1565
#define FI_POWERPOINTMACB3      1566
#define FI_POWERPOINTMACB4      1567

/* These were added for GDSF support */
#define FI_HARVARDWINDOWS       1568    /* Harvard Graphics for Windows */
#define FI_IGES                 1569    /* IGES Drawing File Format */
#define FI_IBMPIF               1570    /* IBM Picture Interchange Format */
#define FI_XBITMAP              1571    /* X-Windows Bitmap */
#define FI_XPIXMAP              1572    /* X-Windows Pixmap */
#define FI_CALSRASTER           1573    /* GP4, CALS Raster File Format */
#define FI_PNG                  1574    /* Portable Network Graphics Format */
#define FI_XDUMP                1575    /* X-Windows Dump */
#define FI_CORELDRAWCLIPART     1576
#define FI_HPGALLERY            1577
#define FI_GDF                  1578    /* Graphics Data Format */
#define FI_DESIGNER             1579
#define FI_POSTSCRIPT           1580
/* end of GDSF additions */

#define FI_POWERPOINT97         1581
#define FI_CORELDRAW6           1582
#define FI_CORELDRAW7           1583
#define FI_PDFMACBIN            1584
#define FI_AUTOCADDWG           1585
#define FI_VISIO4               1586
#define FI_AUTOCADDWG14         1587
#define FI_PBM                  1588
#define FI_PGM                  1589
#define FI_PPM                  1590
#define FI_ADOBEPHOTOSHOP       1591
#define FI_POWERPOINT9597       1592    /* PowerPoint 95/97 Dual Storage */
#define FI_PAINTSHOPPRO         1593
#define FI_FLASHPIX             1594
#define FI_VISIO5               1595
#define FI_CORELDRAW8           1596
#define FI_VISIO6               1597
#define FI_CORELDRAW9           1598
#define FI_PROGRESSIVEJPEG      1599
#define FI_POWERPOINT2000       1600    /* PowerPoint 2000 */
#define FI_DGN                  1601
#define FI_BMP5                 1602
#define FI_WBMP                 1603
#define FI_MIFFG                1604
#define FI_POWERPOINT2          1605
#define FI_WPG10                1606
#define FI_VISIO3               1607
#define FI_DESIGNER7            1608
#define FI_PDFIMAGE             1609    /* PDF image filter */
#define FI_STAROFFICEIMPRESS52  1610    /* StarOffice Impress 5.2 */
#define FI_ILLUSTRATOR9         1611    /* AI9 uses the PDF Filter */
#define FI_AUTOCADDWG2000       1612
#define FI_AUTOCADDWG2DOT5      1613
#define FI_AUTOCADDWG2DOT6      1614
#define FI_AUTOCADDWG9          1615
#define FI_AUTOCADDWG10         1616
#define FI_QUARKEXP_MAC30       1617
#define FI_QUARKEXP_MAC31       1618
#define FI_QUARKEXP_MAC32       1619
#define FI_QUARKEXP_MAC33       1620
#define FI_QUARKEXP_MAC40       1621
#define FI_QUARKEXP_WIN33       1622
#define FI_QUARKEXP_WIN40       1623
#define FI_QUARKEXP_WIN50       1624
#define FI_EXPORTIMAGE          1625
#define FI_STAROFFICEDRAW6      1626    /* StarOffice Draw 6 & 7 */
#define FI_STAROFFICEIMPRESS6   1627    /* StarOffice Impress 6 & 7 */
#define FI_JBIG2                1628    /* JBIG2 - embeddings only */
#define FI_CORELDRAW10          1629
#define FI_CORELDRAW11          1630
#define FI_VISIO2003            1631    /*Visio 2003 */
#define FI_STAROFFICEDRAW8      1632    /* StarOffice Draw 8 */
#define FI_STAROFFICEIMPRESS8   1633    /* StarOffice Impress 8 */
#define FI_AUTOCADDWG2004       1634
#define FI_POWERPOINT2007       1635    /* PowerPoint 2007 */
#define FI_MICROSOFT_XPS        1636
#define FI_ENCRYPTED_PPT2007    1637    /* PowerPoint 2007 */
#define FI_AUTOCADDWG2007       1638
#define FI_OS2V2BMP             1639    /* OS/2 version 2 bitmap */
#define FI_SVM                  1640    /* StarView Metafile */
#define FI_EFAX                 1641    /* eFax document */
#define FI_EMBEDDEDBMP          0x85DC  /* INTERNAL USE ONLY!!!!!! */
#define FI_EMBEDDEDTIFF         0x85DD  /* FI_EMBEDDED values are set to the non-embedded */
#define FI_EMBEDDEDGIF          0x85DF  /* option values with the high bit set to 1 */
#define FI_EMBEDDEDJPEGFIF      0x85ff
#define FI_EMBEDDEDPNG          0x8626
#define FI_EMBEDDEDWBMP         0x8643
#define FI_EMBEDDEDJBIG2        0x865C
#define FI_WATERMARK	        0x865D
#define FI_EMBEDDEDJP2			0x866E
#define FI_DRM_POWERPOINT       1642
#define FI_DRM_POWERPOINT2007   1643
#define FI_AUTODESK_DWF		1644
#define FI_CORELDRAW12          1645
#define FI_JPEG2000				1646
#define FI_ADOBEINDESIGN        1647
#define FI_JPEG2000JPF			1648 /*NOT SUPPORTED*/
#define FI_JPEG2000MJ2			1649 /*NOT SUPPORTED*/
#define FI_GRAPHICEND           1649


    /*
    |   Other
    */

#define FI_OTHERBEGIN           1650

#define FI_WPINFORMS1           1650
#define FI_SNAPSHOTTEXT         1651
#define FI_SNAPSHOTBMP          1652
#define FI_ICF                  1653

    /* Escher is the Microsoft drawing layer.  It is not
     * a separate file type, but is used in conjunction with
     * a host file type to express the drawings in a document.
     *
     * This ID is used by the Escher filter.
     */
#define FI_ESCHER                1654

#define FI_PDFA                  1655  /* Used exclusively by PDF Export as an output type */
#define FI_PDFA_2                1656  /* Used exclusively by PDF Export as an output type */

#define FI_OTHEREND              1656

    /*
    |   Multimedia
    */

#define FI_MULTIMBEGIN          1700

#define FI_RIFFWAVE             1700
#define FI_RIFFAVI              1701
#define FI_MIDI                 1702
#define FI_DIRECTOR             1703
#define FI_FLASH6               1704
#define FI_FLASH                FI_FLASH6       /* For reverse compatability */
#define FI_QUICKTIME            1705

#define FI_MP3_ID31             1706    /* MP3 file with ID3v1.x metadata */
#define FI_MP3_ID32             1707    /* MP3 file with ID3v2.x metadata */
#define FI_ID31                 1708    /* Non-MP3 multimedia file with ID3v1.x metadata */
#define FI_ID32                 1709    /* Non-MP3 multimedia file with ID3v2.x metadata */
#define FI_MP3                  1710    /* MP3 file with no metadata */
#define FI_MPGAV1L1             1711    /* MPEG-1 audio - Layer 1 */
#define FI_MPGAV1L2             1712    /* MPEG-1 audio - Layer 2 */
#define FI_MPGAV2L1             1713    /* MPEG-2 audio - Layer 1 */
#define FI_MPGAV2L2             1714    /* MPEG-2 audio - Layer 2 */
#define FI_MPGAV2L3             1715    /* MPEG-2 audio - Layer 3 */
#define FI_ASF                  1716    /* Advanced Systems Format */
#define FI_WMV                  1717    /* Windows Media Video (ASF subtype) */
#define FI_WMA                  1718    /* Windows Media Audio (ASF subtype) */
#define FI_DVR_MS               1719    /* Microsoft Digital Video Recording (ASF subtype) */
#define FI_REALMEDIA            1720    /* Real Media (both Real Audio and Real Video) */
#define FI_MPEG1                1721    /* MPEG-1 video */
#define FI_MPEG2                1722    /* MPEG-2 video */
#define FI_ISOBASEMEDIAFILE     1723    /* ISO Base Media File Format */
#define FI_MPEG4                1724    /* MPEG-4 file  */
#define FI_MPEG7                1725    /* MPEG-7 file  */
#define FI_RIFF                 1726    /* Resource Interchange File Format file */
#define FI_FLASH9               1727
#define FI_FLASH10              1728
#define FI_MULTIMEND            1728

    /*
    |   Special
    */

#define FI_SPECIALBEGIN         1800

#define FI_EXECUTABLE           1800
#define FI_COM                  1801
#define FI_ZIP                  1802
#define FI_ZIPEXE               1803
#define FI_ARC                  1804
#define FI_BINDER               1805
#define FI_UNIXCOMP             1806
#define FI_TAR                  1807
#define FI_ENVOY                1808
#define FI_QUICKFINDER          1809
#define FI_WINCLIPFILE          1810
#define FI_ENVOY7               1811
#define FI_STUFFIT              1812
#define FI_LZH                  1813
#define FI_LZH_SFX              1814
#define FI_GZIP                 1815
#define FI_JAVACLASS            1816
#define FI_MBOX                 1817
#define FI_NSF6                 1818    /* unsupported */
#define FI_ENCRYPTED_UNKNOWNMSFTOFFICEDOC 1819
#define FI_MSCAB                1820
#define FI_RAR                  1821
#define FI_RAREXE               1822
#define FI_MICROSOFTINFOPATH    1823
#define FI_ONENOTE2007          1824   /* obsolete - replaced by FI_MSONENOTEPACKAGE */
#define FI_MSONENOTEPACKAGE     1824 
#define FI_WPL                  1825   /* FI Only - Windows Media Player Playlist */
#define FI_7Z                   1826   /* 7z archives  */
#define FI_7ZEXE                1827   /* 7z self extracting archives  */
#define FI_MSONENOTETOC         1828
#define FI_MSONENOTE            1829
#define FI_JSON                 1830   /* javascript object notation */
#define FI_OST_2013             1831
#define FI_MSONENOTEHTTP        1832   /*  SOAP/HTTP storage of One Note.  */
#define FI_SPECIALEND           1832

    /*
    |   Test application
    */
#define FI_CHUNKERBINARY        1900
#define FI_TESTQUEUE            1901
#define FI_NULL                 1902    /* Used by the NULL Export filter for testing mem leaks */
#define FI_CATEST               1903    /* Used to test CA. */
#define FI_CHUNKEROUTPUTTEST    1904    /* Used to test Chunker output. */


#define FI_DEFAULT              1995    /* Not a real FI - Used by pipeline components when the protocol supports a single format for which there is no FI id */
#define FI_PASSTHROUGH          1996  /* Not a real FI - Used by pipeline components when their output format will be the same as their input formats */
#define FI_ANY                  1997    /* Not a real FI - Used by pipeline components that support any format */
#define FI_NONE                 1998
#define FI_UNKNOWN              1999


    /*
    |   XML (see also FI_XML and FI_XML_DOCTYPE_HTML)
    */
#define FI_XMLBEGIN             2001

#define FI_XML_ANY              2000  /* Not a real FI - Used by XML pipeline components that support any XML schema */
#define FI_XML_FLEXIONDOC       2001  /* Flexiondoc 1 (original) schema */
#define FI_XML_FLEXIONDOC2      2002  /* Flexiondoc 2 schema */
#define FI_XML_FLEXIONDOC3      2003  /* Flexiondoc 3 schema */
#define FI_XML_FLEXIONDOC4      2004  /* Flexiondoc 4 schema */
#define FI_XML_FLEXIONDOC5      2005  /* Flexiondoc 5 schema */
#define FI_XML_FLEXIONDOC5_1    2006  /* Flexiondoc 5.1 schema */
#define FI_XML_DXL              2007  /* Domino XML schema */
#define FI_XML_FLEXIONDOC5_2    2008  /* Flexiondoc 5.2 schema */
                                      /* 2009 retired */ 
#define FI_XML_ADOBEINX         2010  /* Adobe Indesign Interchange */
#define FI_XML_VISIO            2012
    /*
    |   HTML/Wireless Export component flavors
    |
    |   IDs 2011 - 2029 are reserved for HTML/Wireless Export (FlexionDoc active component)
    */
#define FI_ML                   2011


#define FI_DXL_MAILARCHIVE      2015    /* Mail archive DXL */
#define FI_DXL_MAILMESSAGE      2016    /* Mail message DXL */
#define FI_DXL_GENERIC          2017    /* Generic DXL */
#define FI_DXL_MAILRULE			2018    /* Mail Rule DXL    */


/* #define FI_HTMLRESERVED_LAST 2029 */
#define FI_XML_FLEXIONDOC5_3     2030  /* Flexiondoc 5.3 schema */
#define FI_XML_OPENDOCUMENT_1_0  2031  /* Reserved */
#define FI_XML_FLEXIONDOC5_4     2032  /* Flexiondoc 5.4 schema */
#define FI_XML_FLEXIONDOC_LATEST 2033 /* This FI will always be recognized by 
                                         the export API as referring to the
                                         most recent FlexionDoc schema. This 
                                         literal value does not change when Flexion's
                                         schema is revised. */
#define FI_XML_FLEXIONDOC5_5     2034  /* Flexiondoc 5.5 schema */
#define FI_XML_FLEXIONDOC5_6     2035  /* Flexiondoc 5.6 schema */
#define FI_XML_FLEXIONDOC5_7     2036  /* Flexiondoc 5.7 schema */

#define FI_XML_FLEXIONDOC_CURRENT  FI_XML_FLEXIONDOC5_7 /* This is an internal alias for the 
                                                           current FlexionDoc schema version. 
                                                           It is updated when the FlexionDoc 
                                                           schema is revised.*/

#define FI_XML_BITFORM		2090
#define FI_XMLEND               2099


/*
|  New Section for graphics
*/
#define FI_GRAPHIC2BEGIN        2200
#define FI_AUTOCADDWG2008       2200
#define FI_PUBLISHER2003        2201
#define FI_PUBLISHER2007        2202
#define FI_OPENOFFICEIMPRESS1   2203    /* Open Office Impress 6 */
#define FI_OPENOFFICEIMPRESS2   2204    /* Open Office Impress 8 */
#define FI_SYMPHONYPRESENTATION 2205    /* IBM Lotus Symphony Presentations */
#define FI_OPENOFFICEDRAW1      2206    /* Open Office Draw 6 */
#define FI_OPENOFFICEDRAW2      2207    /* Open Office Draw 8 */
#define FI_POWERPOINTTEMPLATE2007       2208    /* PowerPoint 2007 Template*/
#define FI_POWERPOINT2007_MACRO         2209    /* PowerPoint 2007 Macro Enabled*/
#define FI_POWERPOINTTEMPLATE2007_MACRO  2210   /* PowerPoint 2007 Template Macro Enabled*/
#define FI_POWERPOINTSLIDESHOW2007       2211   /* PowerPoint 2007 Slideshow file */
#define FI_POWERPOINTSLIDESHOW2007_MACRO 2212   /* PowerPoint 2007 Template Macro Enabled*/
#define FI_RPIX					2213	/* Oracle Multimedia internal raster format */
#define FI_OPENOFFICEIMPRESS3   2214    /* Open Office Impress 8 */
#define FI_OPENOFFICEDRAW3      2215    /* Open Office Draw 8 */
#define FI_PRESENTATIONS_X4     2216    /* Corel presentations X4 */
#define FI_ACCESSSNAPSHOT       2217
#define FI_STAROFFICEIMPRESS9   2218
#define FI_STAROFFICEDRAW9      2219
#define FI_ORACLEOPENOFFICEIMPRESS3 2220
#define FI_ORACLEOPENOFFICEDRAW3    2221
#define FI_POWERPOINT2010            2222
#define FI_POWERPOINTTEMPLATE2010    2223
#define FI_POWERPOINTTEMPLATE2010_MACRO 2224
#define FI_POWERPOINTSLIDESHOW2010   2225
#define FI_POWERPOINT2010_MACRO      2226
#define FI_POWERPOINTSLIDESHOW2010_MACRO  2227
#define FI_ENCRYPTED_PPT2010         2228
#define FI_AUTOCADDWG2010       2229
#define FI_PRESENTATIONS_X5     2230   /* Corel presentations X5 */
#define FI_IWORK_KEYNOTE             2231
#define FI_IWORK_KEYNOTE_PREVIEW     2232
#define FI_SVG                       2233 /* Scalable Vector Graphics */
#define FI_AUTODESK_DWF_ARCHIVED 		 2234

#define FI_POWERPOINT2013            2235
#define FI_POWERPOINTTEMPLATE2013    2236
#define FI_POWERPOINTTEMPLATE2013_MACRO 2237
#define FI_POWERPOINTSLIDESHOW2013   2238
#define FI_POWERPOINT2013_MACRO      2239
#define FI_POWERPOINTSLIDESHOW2013_MACRO  2240

#define FI_OFFICETHEME               2241
#define FI_ADOBEPHOTOSHOP_PSB        2242
#define FI_DICOM                     2243
#define FI_PRESENTATIONS_X6          2244
#define FI_AUTOCADDWG2013       2245
#define FI_HARVARDGRAPHICS98    2246  /* fw 5/9/13 - (BugDB # 12608955) */
#define FI_VISIO2013            2247
#define FI_VISIOTEMPLATE2013    2248
#define FI_VISIOSTENCIL2013     2249
#define FI_VISIO2013_MACRO            2250
#define FI_VISIOTEMPLATE2013_MACRO    2251
#define FI_VISIOSTENCIL2013_MACRO     2252

#define FI_LIBREOFFICEIMPRESS3 2253
#define FI_LIBREOFFICEDRAW3    2254
#define FI_LIBREOFFICEIMPRESS4 2255
#define FI_LIBREOFFICEDRAW4    2256

#define FI_APACHEOFFICEIMPRESS3 2257
#define FI_APACHEOFFICEDRAW3    2258
#define FI_APACHEOFFICEIMPRESS4 2259
#define FI_APACHEOFFICEDRAW4    2260

#define FI_TIFFANDJPG           2261    /* TIFF unless color (and optionally grayscale), then JPEG */

#define FI_POWERPOINT2016            2262
#define FI_POWERPOINTTEMPLATE2016    2263
#define FI_POWERPOINTTEMPLATE2016_MACRO 2264
#define FI_POWERPOINTSLIDESHOW2016   2265
#define FI_POWERPOINT2016_MACRO      2266
#define FI_POWERPOINTSLIDESHOW2016_MACRO  2267

#define FI_PRESENTATIONS_X7     2268

#define FI_STRICTXML_POWERPOINT2013     2269
#define FI_STRICTXML_POWERPOINT2016     2270
#define FI_CORELDRAWX           2271
#define FI_WEBP                 2272
#define FI_IWORK13_KEYNOTE      2273
#define FI_CORELDRAWX_TEMPLATE  2274
#define FI_GRAPHIC2END          2275



/*
|   section for internal IDs
*/
#define FI_OITINTERNALBEGIN     2300
#define FI_OITINTERNALEND       2399

#define FI_PERFORMANCE			(FI_OITINTERNALBEGIN + 0)
#define FI_JPXINPDF				(FI_OITINTERNALBEGIN + 1)


#define FI_SYSTEMBEGIN          2400
#define FI_WIN_EXPLORERCMD      2400
#define FI_WIN_SHORTCUT         2401
#define FI_WINDOWSHELP          2402
#define FI_WIN_COMPILEDHELP     2403
#define FI_TRUETYPEFONT         2404
#define FI_TRUETYPECOLLECTION   2405
#define FI_TRUETYPEFONT_MAC     2406
#define FI_SYSTEMEND            2499


/*
|   Spreadsheets (Continued)
*/
#define FI_SS2BEGIN                  2500
#define FI_EXCEL2016                 2500
#define FI_EXCEL2016_MACRO           2501
#define FI_EXCELTEMPLATE2016         2502 
#define FI_EXCELTEMPLATE2016_MACRO   2503
#define FI_EXCEL2016_ADDINMACRO      2504
#define FI_EXCEL2016_BINARY          2505
#define FI_QUATTROPROX7NB            2506

#define FI_STRICTXML_EXCEL2013     2507
#define FI_STRICTXML_EXCEL2016     2508
#define FI_IWORK13_NUMBERS         2509

#define FI_SS2END                    2509

    /*
    |    Reserved (Writer's Toolkit)
    */

#define FI_RESERVEDBEGIN        2990 /* Was 1600 to ... */

#define FI_TKTHESAURUS          2990
#define FI_TKABBREV             2991
#define FI_TKDICTIONARY         2992
#define FI_TKQUOTE              2993
#define FI_TKWRITTENWORD        2994
#define FI_TKCULTURELIT         2995
#define FI_TKGRAMMAR            2996
#define FI_TKTHESSYN            2997

#define FI_RESERVEDEND          2997 /* ... 1607 */


    /*
    | Range of FIs reserved for OEM use
    */

#define FI_OEMBEGIN             3000
#define FI_OEMEND               3999

    /*
    |
    |   Non-standard file identification codes reserved by SCC
    |
    */

    /*
    |   Codes used to force generic views and specific encodings
    */

#define FI_GENERICBEGIN         4000

#define FI_ASCII                4000    /* Text - ASCII - 7bit */
#define FI_HEX                  4001    /* Hex */
#define FI_ANSI                 4002    /* Text - ANSI - 7bit */
#define FI_UNICODE              4003    /* Text - UNICODE */
#define FI_ASCII8               4004    /* Text - ASCII - 8bit */
#define FI_ANSI8                4005    /* Text - ANSI - 8bit */
#define FI_DONTVIEW             4006
#define FI_MAC                  4007    /* Text - MAC - 7bit */
#define FI_MAC8                 4008    /* Text - MAC - 8bit */
#define FI_SHIFTJIS             4009
#define FI_CHINESEGB            4010
#define FI_HANGEUL              4011
#define FI_CHINESEBIG5          4012
#define FI_LATIN2               4013    /* Code page 852 - MS DOS Slavic */
#define FI_JAPANESE_EUC         4014


/*** Bidi ***/
#define FI_HEBREW_OLDCODE       4015
#define FI_HEBREW_PC8           4016
#define FI_HEBREW_E0            4017
#define FI_HEBREW_WINDOWS       4018
#define FI_ARABIC_710           4019
#define FI_ARABIC_720           4020
#define FI_ARABIC_WINDOWS       4021

#define FI_7BITTEXT             4022    /* Used by the extended FI to ID text */

#define FI_JAPANESE_JIS                      4023
#define FI_CENTRALEU_1250                    4024
#define FI_UTF8                              4025
#define FI_EBCDIC_37                         4026  /* U.S. English/Portuguese EBCDIC Text                      */
#define FI_EBCDIC_273                        4027  /* Austrian/German EBCDIC Text                              */
#define FI_EBCDIC_277                        4028  /* Danish/Norwegian EBCDIC Text                             */
#define FI_EBCDIC_278                        4029  /* Finnish/Swedish EBCDIC Text                              */
#define FI_EBCDIC_280                        4030  /* Italian EBCDIC Text                                      */
#define FI_EBCDIC_284                        4031  /* Spanish EBCDIC Text                                      */
#define FI_EBCDIC_285                        4032  /* U.K. English EBCDIC Text                                 */
#define FI_EBCDIC_297                        4033  /* French EBCDIC Text                                       */
#define FI_EBCDIC_500                        4034  /* Belgian/International EBCDIC Text                        */
#define FI_EBCDIC_870                        4035  /* Hungarian/Polish/Yugoslavian/Czechoslovakian EBCDIC Text */
#define FI_EBCDIC_871                        4036  /* Icelandic EBCDIC Text                                    */
#define FI_EBCDIC_1026                       4037  /* Turkish EBCDIC Text                                      */
#define FI_HTML_EBCDIC_37                    4038  /* U.S. English/Portuguese EBCDIC HTML                      */
#define FI_HTML_EBCDIC_273                   4039  /* Austrian/German IBM EBCDIC HTML                          */
#define FI_HTML_EBCDIC_277                   4040  /* Danish/Norwegian IBM EBCDIC HTML                         */
#define FI_HTML_EBCDIC_278                   4041  /* Finnish/Swedish IBM EBCDIC HTML                          */
#define FI_HTML_EBCDIC_280                   4042  /* Italian IBM EBCDIC HTML                                  */
#define FI_HTML_EBCDIC_284                   4043  /* Spanish IBM EBCDIC HTML                                  */
#define FI_HTML_EBCDIC_285                   4044  /* U.K. English IBM EBCDIC HTML                             */
#define FI_HTML_EBCDIC_297                   4045  /* French IBM EBCDIC HTML                                   */
#define FI_HTML_EBCDIC_500                   4046  /* Belgian/International IBM EBCDIC HTML                    */
#define FI_HTML_EBCDIC_870                   4047  /* Hungarian/Polish/Yugoslavian/Czechoslovakian EBCDIC HTML */
#define FI_HTML_EBCDIC_871                   4048  /* Icelandic IBM EBCDIC HTML                                */
#define FI_HTML_EBCDIC_1026                  4049  /* Turkish IBM EBCDIC HTML                                  */
#define FI_UUENCODEDTEXT                     4050
#define FI_UUENCODEDPART                     4051
#define FI_XXENCODEDTEXT                     4052
#define FI_XXENCODEDPART                     4053
#define FI_YENCODEDTEXT                      4054
#define FI_YENCODEDPART                      4055
#define FI_BINHEXENCODEDTEXT                 4056
#define FI_BINHEXENCODEDPART                 4057
#define FI_ARABIC_ASMO708                    4058  /* Arabic Text                ASMO-708                      */
#define FI_ARABIC_DOS                        4059  /* Arabic Text                DOS OEM 720 TRANSPARENT ASMO  */
#define FI_ARABIC_ISO                        4060  /* Arabic Text                ISO 8859-6                    */
#define FI_ARABIC_MAC                        4061  /* Arabic Text                Mac                           */
#define FI_BALTIC_ISO                        4062  /* Baltic Text                ISO 8859-4                    */
#define FI_BALTIC_WINDOWS                    4063  /* Baltic Text                Windows ANSI 1257             */
#define FI_CENTRALEUROPEAN_DOS               4064  /* Central European Text      DOS OEM 852 Latin II          */
#define FI_CENTRALEUROPEAN_ISO               4065  /* Central European Text      ISO 8859-2                    */
#define FI_CENTRALEUROPEAN_MAC               4066  /* Central European Text      Mac                           */
#define FI_CENTRALEUROPEAN_WINDOWS           4067  /* Central European Text      Windows ANSI 1250             */
#define FI_CHINESESIMPLIFIED_WINDOWS         4068  /* Chinese Simplified Text    Windows ANSI 936 (GB2312)     */
#define FI_CHINESETRADITIONAL_WINDOWS        4069  /* Chinese Traditional Text   Windows ANSI 950 (BIG5)       */
#define FI_CYRILLIC_DOS                      4070  /* Cyrillic Text              DOS OEM 855                   */
#define FI_CYRILLIC_ISO                      4071  /* Cyrillic Text              ISO 8859-5                    */
#define FI_CYRILLIC_KOI8R                    4072  /* Cyrillic Text              KOI8-R                        */
#define FI_CYRILLIC_MAC                      4073  /* Cyrillic Text              Mac                           */
#define FI_CYRILLIC_WINDOWS                  4074  /* Cyrillic Text              Windows ANSI 1251             */
#define FI_GREEK_ISO                         4075  /* Greek Text                 ISO 8859-7                    */
#define FI_GREEK_MAC                         4076  /* Greek Text                 Mac                           */
#define FI_GREEK_WINDOWS                     4077  /* Greek Text                 Windows ANSI 1253             */
#define FI_HEBREW_DOS                        4078  /* Hebrew Text                DOS OEM 862                   */
#define FI_HEBREW_ISO_VISUAL                 4079  /* Hebrew Text                ISO 8859-8                    */
#define FI_JAPANESE_MAC                      4080  /* Japanese Text              Mac                           */
/* DEPRECATED - #define FI_JAPANESE_SHIFTJIS  4081 - Japanese Text               Windows Shift-JIS ANSI 932    */
#define FI_KOREAN_JOHAB                      4082  /* Korean Text                Windows ANSI 1361 (Johab)     */
#define FI_KOREAN_WINDOWS                    4083  /* Korean Text                Windows ANSI 949              */
#define FI_RUSSIAN_DOS                       4084  /* Russian Text               DOS OEM 866                   */
#define FI_THAI_WINDOWS                      4085  /* Thai Text                  Windows ANSI 874              */
#define FI_TURKISH_DOS                       4086  /* Turkish Text               DOS OEM 857                   */
#define FI_TURKISH_ISO                       4087  /* Turkish Text               ISO 8859-9                    */
#define FI_TURKISH_MAC                       4088  /* Turkish Text               Mac                           */
#define FI_TURKISH_WINDOWS                   4089  /* Turkish Text               Windows ANSI 1254             */
#define FI_VIETNAMESE_WINDOWS                4090  /* Vietnamese Text            Windows ANSI 1258             */
#define FI_WESTERNEUROPEAN_ISO               4091  /* Western European Text      ISO 8859-1                    */
#define FI_WESTERNEUROPEAN_MAC               4092  /* Western European Text      Mac                           */
#define FI_WESTERNEUROPEAN_WINDOWS           4093  /* Western European Text      Windows ANSI 1252             */
#define FI_HTML_ARABIC_ASMO708               4094  /* Arabic HTML                ASMO-708                      */
#define FI_HTML_ARABIC_DOS                   4095  /* Arabic HTML                DOS OEM 720 TRANSPARENT ASMO  */
#define FI_HTML_ARABIC_ISO                   4096  /* Arabic HTML                ISO 8859-6                    */
#define FI_HTML_ARABIC_MAC                   4097  /* Arabic HTML                Mac                           */
#define FI_HTML_ARABIC_WINDOWS               4098  /* Arabic HTML                Windows ANSI 1256             */
#define FI_HTML_BALTIC_ISO                   4099  /* Baltic HTML                ISO 8859-4                    */
#define FI_HTML_BALTIC_WINDOWS               4100  /* Baltic HTML                Windows ANSI 1257             */
#define FI_HTML_CENTRALEUROPEAN_DOS          4101  /* Central European HTML      DOS OEM 852 Latin II          */
#define FI_HTML_CENTRALEUROPEAN_ISO          4102  /* Central European HTML      ISO 8859-2                    */
#define FI_HTML_CENTRALEUROPEAN_MAC          4103  /* Central European HTML      Mac                           */
#define FI_HTML_CENTRALEUROPEAN_WINDOWS      4104  /* Central European HTML      Windows ANSI 1250             */
#define FI_HTML_CHINESESIMPLIFIED_EUC        4105  /* Chinese Simplified HTML    EUC                           */
#define FI_HTML_CHINESESIMPLIFIED_WINDOWS    4106  /* Chinese Simplified HTML    Windows ANSI 936 (GB2312)     */
#define FI_HTML_CHINESETRADITIONAL_WINDOWS   4107  /* Chinese Traditional HTML   Windows ANSI 950 (BIG5)       */
#define FI_HTML_CYRILLIC_DOS                 4108  /* Cyrillic HTML              DOS OEM 855                   */
#define FI_HTML_CYRILLIC_ISO                 4109  /* Cyrillic HTML              ISO 8859-5                    */
#define FI_HTML_CYRILLIC_KOI8R               4110  /* Cyrillic HTML              KOI8-R                        */
#define FI_HTML_CYRILLIC_MAC                 4111  /* Cyrillic HTML              Mac                           */
#define FI_HTML_CYRILLIC_WINDOWS             4112  /* Cyrillic HTML              Windows ANSI 1251             */
#define FI_HTML_GREEK_ISO                    4113  /* Greek HTML                 ISO 8859-7                    */
#define FI_HTML_GREEK_MAC                    4114  /* Greek HTML                 Mac                           */
#define FI_HTML_GREEK_WINDOWS                4115  /* Greek HTML                 Windows ANSI 1253             */
#define FI_HTML_HEBREW_DOS                   4116  /* Hebrew HTML                DOS OEM 862                   */
#define FI_HTML_HEBREW_ISO_VISUAL            4117  /* Hebrew HTML                ISO 8859-8                    */
#define FI_HTML_HEBREW_WINDOWS               4118  /* Hebrew HTML                Windows ANSI 1255             */
#define FI_HTML_JAPANESE_MAC                 4119  /* Japanese HTML              Mac                           */
#define FI_HTML_JAPANESE_SHIFTJIS            4120  /* Japanese HTML              Windows Shift-JIS ANSI 932    */
#define FI_HTML_KOREAN_JOHAB                 4121  /* Korean HTML                Windows ANSI 1361 (Johab)     */
#define FI_HTML_KOREAN_WINDOWS               4122  /* Korean HTML                Windows ANSI 949              */
#define FI_HTML_RUSSIAN_DOS                  4123  /* Russian HTML               DOS OEM 866                   */
#define FI_HTML_THAI_WINDOWS                 4124  /* Thai HTML                  Windows ANSI 874              */
#define FI_HTML_TURKISH_DOS                  4125  /* Turkish HTML               DOS OEM 857                   */
#define FI_HTML_TURKISH_ISO                  4126  /* Turkish HTML               ISO 8859-9                    */
#define FI_HTML_TURKISH_MAC                  4127  /* Turkish HTML               Mac                           */
#define FI_HTML_TURKISH_WINDOWS              4128  /* Turkish HTML               Windows ANSI 1254             */
#define FI_HTML_VIETNAMESE_WINDOWS           4129  /* Vietnamese HTML            Windows ANSI 1258             */
#define FI_HTML_WESTERNEUROPEAN_ISO          4130  /* Western European HTML      ISO 8859-1                    */
#define FI_HTML_WESTERNEUROPEAN_MAC          4131  /* Western European HTML      Mac                           */
#define FI_HTML_WESTERNEUROPEAN_WINDOWS      4132  /* Western European HTML      Windows ANSI 1252             */

#define FI_GENERICEND                        4132


#define FI_PLUGIN                            4750

/* for backwards compatibility  */
#define FI_JAPANESE_SHIFTJIS                 FI_SHIFTJIS

    /*
    |   Bitmap/Vector formats stored internally by applications
    */

#define FI_BITMAPBEGIN       5000

#define FI_BINARYMETAFILE    5000   /* Word for Windows internal metafile */
#define FI_WPFWPG            5001   /* WordPerfect internal WPG, no header */
#define FI_AMISNAPSHOT       5002   /* Ami internal snap shot */
#define FI_WORDSNAPSHOT      5003   /* Word internal snap shoot */
#define FI_BINARYMACPICT     5004   /* Macintosh PICT without 512 byte header */
#define FI_BINARYMETABMP     5005   /* Word for Windows internal metafile bitmap */
#define FI_BINARYMETAPICT    5006   /* Word for Windows 6 internal metafile  WITH metfilepict header */
#define FI_WPFWORKSEMBED     5007   /* PerfectWorks internal embedding */
#define FI_WPG2EMBED         5008   /* Bitmap embedded in a WPG2 file */
#define FI_WINDOWSDIB        5009   /* Bitmap w/o BITMAPFILEHEADER in WKS */
#define FI_WPG1EMBED         5010   /* Bitmap embedded in a WPG1 file */
#define FI_GDSFEMBED         5011   /* Bitmap embedded in another format handled by GDSF */
#define FI_WINBMPCOREINFO    5012   /* Bitmap embedded in Excel with just BITMAPCOREINFO as a header */
#define FI_IAFBWEMBED        5013   /* Black & White Bitmap embedded in Interleaf */
#define FI_IAFCOLOREMBED     5014   /* Color Bitmap embedded in Interleaf */
#define FI_PICTEMBED         5015   /* Bitmap embedded in a PICT file */
#define FI_BITMAPEND         5015

#define FI_OCRTEXT           5100   /* Export OCR data as Text, no formatting*/
#define FI_OCRRTF            5101   /* Export OCR data as RTF, yes formatting*/
#define FI_OCRHTML           5102   /* Export OCR data as HTML.*/
#define FI_IDEND             5102

#define FI_EDRM1             5200   /* EDRM export */

	/*
	|  OI Interface formats
	*/
#define FI_OIFACEBEGIN       6000

#define FI_OIACTIVECOMPONENT 6000   /* iface/OIActiveComponent.h */
#define FI_OIDOCUMENT        6001   /* iface/OIDocument.h */
#define FI_OIVECT            6002   /* iface/OIVect.h */
#define FI_OISIMPLEVECT      6003   /* iface/OISimpleVect.h */
#define FI_OIVECT3D          6004   /* iface/OIVect3D.h */
#define FI_OITEXT            6005   /* iface/OIText.h */
#define FI_OICHART           6006   /* iface/OIChart.h */
#define FI_OISMARTART        6007   /* iface/OISmartArt.h */
#define FI_OICHUNKERINFO     6008   /* iface/OIChunkerInfo.h */
#define FI_OIVECTORHOTSPOTS  6009   /* iface/OIVectorHotspots.h */
#define FI_OICHUNKER         6010   /* iface/OIChunker.h */
#define FI_OIFILTER          6011   /* iface/OIFilter.h */

#define FI_OIWORDPROCSECTION     6013 /* iface/OIChunkerSection.h for SO_PARAGRAPHS */
#define FI_OISPREADSHEETSECTION  6014 /* iface/OIChunkerSection.h for SO_CELLS */
#define FI_OIDATABASESECTION     6015 /* iface/OIChunkerSection.h for SO_FIELDS */
#define FI_OIBITMAPSECTION       6016 /* iface/OIChunkerSection.h for SO_BITMAP */
#define FI_OIARCHIVESECTION      6017 /* iface/OIChunkerSection.h for SO_ARCHIVE */
#define FI_OIVECTORSECTION       6018 /* iface/OIChunkerSection.h for SO_VECTOR */
#define FI_OIPRESENTATIONSECTION 6019 /* iface/OIChunkerSection.h for SO_PRESENTATION */
#define FI_OICHARTSECTION        6020 /* iface/OIChunkerSection.h for SO_CHART */
#define FI_OIMULTIMEDIASECTION   6021 /* iface/OIChunkerSection.h for SO_MULTIMEDIA */
#define FI_OIEMAILARCHIVESECTION 6022 /* iface/OIChunkerSection.h for SO_EMAILARCHIVE */
#define FI_OIEMAILDOCSECTION     6023 /* iface/OIChunkerSection.h for SO_EMAILDOC */
#define FI_OIHTMLDOCSECTION      6024 /* iface/OIChunkerSection.h for SO_HTMLDOC */


#define FI_OIHTML5           6025   /* iface/OIHTML5.h */
#define FI_OICSS             6026   /* iface/OICSS.h */
#define FI_OISVG             6027   /* iface/OISVG.h */
#define FI_OIDRAWTORECT      6028   /* iface/OIDrawToRect.h */
#define FI_OIFONTMAP         6029   /* iface/OIFontMap.h */
#define FI_OIFRAMEWRAPPER    6030   /* iface/OIFrameWrapper.h */
#define FI_OIHTML5DOCUMENT   6031   /* iface/OIHTML5Document.h */
#define FI_OIIMAGEEXPORT     6032   /* iface/OIImageExport.h */
#define FI_OIIMAGEWRITER     6033   /* iface/OIImageWriter.h */
#define FI_OISPREADSHEET     6034   /* iface/OISpreadsheet.h */
#define FI_OISTYLEMAP        6035   /* iface/OIStyleMap.h */
#define FI_OITEXTOUT         6036   /* iface/OITextOut.h */
#define FI_OITREE            6037   /* iface/OITree.h */

#define FI_OITEXTINFO        6038   /* iface/OITextInfo.h */
#define FI_OIMETADATA        6039   /* iface/OIMetadata.h */
#define FI_OILOGLISTENER     6040   /* util/Log.h */
#define FI_OIRAWTEXT         6041   /* iface/OIRawText.h */
#define FI_OIEMAIL           6042   /* iface/OIEmail.h */
#define FI_OIRAWTEXTSECTION  6043   /* iface/OIChunkerDocument.h for rawtext */
#define FI_OIGENCONTEXT      6044   /* iface/OIGenContext.h */
#define FI_OIKEEPALIVE       6045   /* iface/OIKeepalive.h */
#define FI_OISPEAKERNOTES    6046   /* iface/OISpeakerNotes.h */

#define FI_OIFACEEND         6046




    /*
    |
    |   Entry points
    |
    */

#ifndef FI_ENTRYSC

#ifdef WINDOWS
#define SCCFI_DLL(a)     "SC"#a"FI.DLL"
#endif

#ifdef WIN32
#define FI_ENTRYSC    __declspec(dllexport)
#define FI_ENTRYMOD   __cdecl
#endif /*WIN32*/

#ifdef UNIX
#define FI_ENTRYSC
#define FI_ENTRYMOD
#endif    /*UNIX*/

#ifdef NLM
#define FI_ENTRYSC
#define FI_ENTRYMOD
#endif    /*NLM*/

#endif /* FI_ENTRYSC */

typedef struct FIGETtag
{
    VTWORD wId;
} FIGET, * PFIGET;

#define  FIFLAG_NORMAL				0
#define  FIFLAG_EXTENDEDFI			1
#define  FIFLAG_OVERRIDEOPTIONS		2

FI_ENTRYSC VTWORD FI_ENTRYMOD FIIdFileEx(VTDWORD, VTVOID *, VTDWORD, VTWORD *,VTLPTSTR, VTWORD);
FI_ENTRYSC VTBOOL FI_ENTRYMOD FIGetFirstId(PFIGET, VTWORD *, VTLPTSTR, VTWORD);
FI_ENTRYSC VTBOOL FI_ENTRYMOD FIGetNextId(PFIGET, VTWORD *, VTLPTSTR, VTWORD);
FI_ENTRYSC VTWORD FI_ENTRYMOD FIGetIDString(VTWORD, VTLPTSTR, VTWORD);

FI_ENTRYSC VTWORD FI_ENTRYMOD FIIdSpecial(HIOFILE, VTWORD);

FI_ENTRYSC VTWORD FI_ENTRYMOD FIIdFile(VTDWORD,VTVOID *,VTDWORD,VTWORD *);
FI_ENTRYSC VTSHORT FI_ENTRYMOD FIIdHandle(HIOFILE, VTDWORD, VTWORD *);
#ifdef UNIX_PTHREAD
FI_ENTRYSC VTLONG FI_ENTRYMOD FIThreadInit(VTSHORT ThreadOption);
FI_ENTRYSC VTLONG FI_ENTRYMOD FIThreadInitExt(VTLONG (*Lock)(VTVOID *), VTLONG (*Unlock)(VTVOID *));
#endif
FI_ENTRYSC VTDWORD FI_ENTRYMOD FIInit(VTVOID);
FI_ENTRYSC VTDWORD FI_ENTRYMOD FIDeInit(VTVOID);

/* defines for thread initialization.  Needed only for UNIX */
#define FITHREAD_INIT_PTHREADS  134
#define FITHREAD_INIT_NOTHREADS 135
#define FITHREAD_INIT_NATIVETHREADS 136

#define FITHREAD_INIT_SUCCESS 0
#define FITHREAD_INIT_FAILED  1
#define FITHREAD_INIT_ALREADY_CALLED 2
#define FITHREAD_INIT_NOT_SUPPORTED   3

#ifdef __cplusplus
}
#endif

#ifdef SCC_PACKED_BY_SCCFI_H
#pragma pack(pop)
#undef SCC_PACKED_BY_SCCFI_H
#undef SCC_PACK_SET
#endif /* SCC_PACKED_BY_SCCFI_H */

#endif /*SCCFI_H*/
