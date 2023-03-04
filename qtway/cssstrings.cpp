#include "cssstrings.h"

// Copyright 2013-2021 Pasquale J. Rinaldi, Jr.
// Distributed under the terms of the GNU General Public License version 2

QString ReturnCssString(qint8 cssid)
{
    QString bodycss = "font-size: 12px; font-family: verdana, sans; font-color: #3a291a;";
    QString infotitlecss = "text-transform: uppercase; font-size: 18px; font-family: verdana, sans; font-color: #9b7d4b; font-color: #3a291a;";
    QString tablecss = "border-collapse: collapse; border-color: #644813;";
    QString trcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left;";
    QString troddcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d6ceb5;";
    QString trevencss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d5e8e2;";
    QString thcss = "background-color: #ad9f68; text-transform: uppercase; font-size: 12px; font-weight: bold; font-family: verdana, sans; padding: 5px 10px 5px 10px; border-bottom: 2px solid #3a291a;";
    QString tdcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px;";
    QString tdavalcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:150px;";
    QString tdavalvtopcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:150px; vertical-align: top;";
    QString tdsubheadcss = "background-color: #ad9f68; text-transform: uppercase; font-size: 12px; font-weight: bold; font-family: verdana, sans; padding: 5px 10px 5px 10px; border-bottom: 2px solid #3a291a;";
    QString tdfitemcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:150px; vertical-align: top;";
    QString troddvtopcss = "text-transform: uppercase; font-family: verdana, sans; font-size: 12px; text-align: left; background-color: #d6ceb5; vertical-align: top;";
    QString tdpvaluecss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:72px";
    QString tdpropcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; width:188px";
    QString tdvtopcss = "padding-left: 10px; padding-top: 2.5px; padding-right: 10px; padding-bottom: 2.5px; vertical-align: top;";

    switch(cssid)
    {
        case 0:
            return bodycss;
        case 1:
            return infotitlecss;
        case 2:
            return tablecss;
        case 3:
            return trcss;
        case 4:
            return troddcss;
        case 5:
            return trevencss;
        case 6:
            return thcss;
        case 7:
            return tdcss;
        case 8:
            return tdavalcss;
        case 9:
            return tdavalvtopcss;
        case 10:
            return tdsubheadcss;
        case 11:
            return tdfitemcss;
        case 12:
            return troddvtopcss;
        case 13:
            return tdpvaluecss;
        case 14:
            return tdpropcss;
        case 15:
            return tdvtopcss;
    }
}
