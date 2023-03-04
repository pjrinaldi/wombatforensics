/*
|   HTML/Image/Search/XML Export
|   Sample Application
|
|   Copyright (c) 2001, 2016, Oracle and/or its affiliates. All rights reserved.
|   All rights reserved.
|
|   You have a royalty-free right to use, modify, reproduce and
|   distribute the Sample Applications (and/or any modified version)
|   in any way you find useful, provided that you agree that Oracle
|   has no warranty obligations or liability for any Sample Application
|   files which are modified.
*/

#ifdef WIN32                /* Defined by my Win32 compiler */
#define     WINDOWS         /* Required for Windows */
#include    <windows.h>
#else
/*  Define UNIX if it has not already been defined.  */
#ifndef UNIX
#define     UNIX            /* Required for UNIX */
#endif
#endif

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include	<float.h>
#ifndef UNIX
#include	<scctype.h>
#include    <tchar.h>
#include    <io.h>
#else
#include    <unistd.h>
#endif
#include	<float.h>
#include    "sccex.h"       /* Required */
#include    "cfg.c"         /* User interface code for this app, NOT needed by export. */

#ifdef UNIX
/*
   Oracle does version tracking of the Outside In sample applications.
   OIT_VERSTRING is defined by internal build scripts so you can delete
   the following line at your leisure as it performs no real function.
*/
#ifdef OIT_VERSTRING
char oitsamplever[32] = OIT_VERSTRING;
#endif

#define PATH_TYPE   IOTYPE_UNIXPATH
#else
#define PATH_TYPE   IOTYPE_ANSIPATH
#endif

#if 0
#define PRINT_OPTIONS       /* When defined, PrintOpts() prints the values of all the configuration options. */
#endif


/* buffer for error strings */
char _szError[256];
VTDWORD gdwFileCount;               /* Global count of files created. */
VTDWORD gdwGraphicExportErrors;     /* Global count of graphics export failures */
VTDWORD gdwArchiveExportErrors;     /* Global count of archive files export failures */
VTBYTE  szFullName[256];            /* Full name of the archive file */
VTBOOL  gbHandleEndPage = FALSE;    /* Whether or not to handle the endpage callback */

/* Function prototypes */
SCCERR ExportOemCallback(VTHEXPORT hExport, VTSYSPARAM dwCallbackData, VTDWORD dwCommandID, VTLPVOID pCommandData);


int main(int argc, char *argv[])
{
    Option      Opts = {0};       /* Export configuration parameters. Application specific implementation. */
    FILE        *pFile = NULL;
    VTBOOL      bUnicodeCallbackStr = FALSE;
    VTHDOC      hDoc;
    VTHEXPORT   hExport;
    VTLPSTR		pstrFile = (VTLPSTR)((argc == 4 || argc == 5) ? argv[3]: DEFAULT_CONFIG_FILE);
    EXSTATUSINFORMATION infoStatus;
    EXSUBDOCSTATUS status;
    SCCERR      seStatusResult = SCCERR_UNKNOWN; /* initializing to something other that SCCERR_OK */
    VTDWORD     dwDaInitExFlags = OI_INIT_DEFAULT;

    SCCERR seResult = ReadCommandLine(argc, argv, &Opts, &pFile);
    if (seResult != SCCERR_OK)
    {
        goto ErrorExit;
    }

    /*
    |   Read the configuration file and set the global options.  Most
    |   options must be set using the hDoc returned by DAOpenDocument().
    |   However, a couple options we handle such as the FIFLAGS option
    |   require a NULL handle.
    */
    seResult = ParseConfigFile(pFile, &Opts);
    if (seResult != SCCERR_OK)
    {
        goto ErrorExit;
    }

    gdwFileCount = 0;
    gdwGraphicExportErrors = 0;  /* count of # of graphics images not converted successfully */
    gdwArchiveExportErrors = 0;  /* count of # of archive files not converted successfully */

    if((TRUE == Opts.abSetOptions[SAMPLE_OPTION_LOAD_OPTIONS])&&(FALSE == Opts.bLoadOptions))
    {
        dwDaInitExFlags |= OI_INIT_NOLOADOPTIONS;
    }

    if((TRUE == Opts.abSetOptions[SAMPLE_OPTION_SAVE_OPTIONS])&&(FALSE == Opts.bSaveOptions))
    {
        dwDaInitExFlags |= OI_INIT_NOSAVEOPTIONS;
    }

    /* Initialize the Data Access module.  Required */
    seResult = DAInitEx(SCCOPT_INIT_NOTHREADS, dwDaInitExFlags);
    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "DAInitEx() failed: %s (0x%04X)\n", _szError, seResult);
        goto ErrorExit;
    }



    /* This is a required option for all exports.  If it is not set, the config file is bad." */
    if ( Opts.dwOutputID == 0L )
    {
        DADeInit();
        fprintf(stderr, "Error: Problem parsing configuration file %s\n", pstrFile);
        seResult = SCCERR_BADPARAM;
        goto ErrorExit;
    }

    if (Opts.abSetOptions[SAMPLE_OPTION_TEMPDIRECTORY])
    {
        SCCUTTEMPDIRSPEC tdsSpec;
        tdsSpec.dwSize = sizeof(tdsSpec);
        tdsSpec.dwSpecType = IOTYPE_DEFAULT;
        strncpy(tdsSpec.szTempDirName, Opts.szTempDir, SCCUT_FILENAMEMAX);

        if ((seResult = DASetOption((VTHDOC)NULL, SCCOPT_TEMPDIR, (VTLPVOID)&tdsSpec, sizeof(tdsSpec))) != SCCERR_OK)
        {
            DAGetErrorString(seResult, _szError, sizeof(_szError));
            fprintf(stderr, "DASetOption() failed: %s (0x%04X)\n", _szError, seResult);
            DADeInit();
            goto ErrorExit;
        }
    }

    if (Opts.abSetOptions[SAMPLE_OPTION_FIFLAGS])
    {
        if ((seResult = DASetOption((VTHDOC)NULL, SCCOPT_FIFLAGS, (VTLPVOID)&Opts.dwFIFlags, sizeof(VTDWORD))) != SCCERR_OK)
        {
            DAGetErrorString(seResult, _szError, sizeof(_szError));
            fprintf(stderr, "DASetOption() failed: %s (0x%04X)\n", _szError, seResult);
            DADeInit();
            goto ErrorExit;
        }
    }

    if (Opts.abSetOptions[SAMPLE_OPTION_EMAILHEADER])
    {
        if ((seResult = DASetOption((VTHDOC)NULL, SCCOPT_WPEMAILHEADEROUTPUT, (VTLPVOID)&Opts.dwEmailHeader, sizeof(VTDWORD))) != SCCERR_OK)
        {
            DAGetErrorString(seResult, _szError, sizeof(_szError));
            fprintf(stderr, "DASetOption() failed: %s (0x%04X)\n", _szError, seResult);
            DADeInit();
            goto ErrorExit;
        }
    }

    if (Opts.abSetOptions[SAMPLE_OPTION_PERFORMANCEMODE])
    {
        if ((seResult = DASetOption((VTHDOC)NULL, SCCOPT_EX_PERFORMANCEMODE, (VTLPVOID)&Opts.dwPerformanceMode, sizeof(VTDWORD))) != SCCERR_OK)
        {
            DAGetErrorString(seResult, _szError, sizeof(_szError));
            fprintf(stderr, "DASetOption() failed: %s (0x%04X)\n", _szError, seResult);
            DADeInit();
            goto ErrorExit;
        }
    }

    if (Opts.abSetOptions[SAMPLE_OPTION_HANDLEENDPAGE])
    {
        gbHandleEndPage = Opts.bHandleEndPage;
    }

    SetBufferSize((VTHDOC)NULL, &Opts);   /* Set the IO Buffer sizes (if required) before DAOpenDocument() */

    /* Open the document to be converted (given by Opts.szInFile).  Required */
    seResult = DAOpenDocument(&hDoc, PATH_TYPE, (VTLPVOID)Opts.szInFile, 0);
    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "DAOpenDocument(\"%s\") failed: %s (0x%04X)\n", Opts.szInFile,_szError, seResult);
        DADeInit();
        goto ErrorExit;
    }

    /* In versions 7.7 and earlier, the output file needed to be
     * deleted if it already existed.  Now export handles this
     * automatically.
     */

    /* Set the rest of the options */
    seResult = SetOptions(hDoc, &Opts);

    if (seResult == SCCERR_OK)
    {
        /* This sample app is not Unicode callback string enabled at this time. */
        seResult = DASetOption(hDoc, SCCOPT_EX_UNICODECALLBACKSTR,
                               (VTLPVOID)&bUnicodeCallbackStr, sizeof(VTBOOL));
    }

    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "DASetOption() failed: %s (0x%04X)\n", _szError, seResult);
        DACloseDocument(hDoc);
        DADeInit();
        goto ErrorExit;
    }
    PrintOpts(&Opts);

#ifdef UNIX
    if (Opts.bPrintUnixOptionsFile)
    {
        fprintf(stderr, "===> Unix Options File: %s\n", Opts.sUnixOptionsFileInfo.szFName );
    }
#endif

    /* EXOpenExport() on the document.  Required */
    seResult = EXOpenExport(hDoc, Opts.dwOutputID, PATH_TYPE, Opts.szOutFile, 0, 0,
                            (EXCALLBACKPROC)ExportOemCallback, 0, &hExport);
    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "EXOpenExport() failed: %s (0x%04X)\n", _szError, seResult);
        DACloseDocument(hDoc);
        DADeInit();
        goto ErrorExit;
    }


    /* Do the actual conversion.  Required */
    seResult = EXRunExport(hExport);

    if (gdwGraphicExportErrors != 0)   /* did we have problems with any graphics conversions? */
    {
        fprintf(stderr, "Failed to convert %d graphic image(s).\n", gdwGraphicExportErrors);
    }

    if (gdwArchiveExportErrors != 0)   /* did we have problems with any archive conversions? */
    {
        fprintf(stderr, "Failed to convert %d archive files(s).\n", gdwArchiveExportErrors);
    }

    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "EXRunExport() failed: %s (0x%04X)\n", _szError, seResult);
        EXCloseExport(hExport);
        DACloseDocument(hDoc);
        DADeInit();
        goto ErrorExit;
    }

    /*  Check status of export.  Only SearchML and XX support this*/
    switch (Opts.dwOutputID)
    {
        case FI_SEARCHML_LATEST:
        case FI_SEARCHML30:
        case FI_SEARCHML31:
        case FI_SEARCHML32:
        case FI_SEARCHML33:
        case FI_SEARCHML34:
        case FI_SEARCHML35:
        case FI_SEARCHHTML:
        case FI_XML_FLEXIONDOC_LATEST:
        case FI_XML_FLEXIONDOC5_5:
            status.dwSize = sizeof(status);
            seStatusResult = EXExportStatus(hExport, EXSTATUS_SUBDOC, &status);
            break;

        default:
            memset(&status, 0, sizeof(status));
    }

    /*If the option is set, get the info struct.*/
    if (Opts.dwExportStatusType == EXSTATUS_INFORMATION)
    {
        infoStatus.dwVersion = EXSTATUSVERSION2;
        seStatusResult = EXExportStatus(hExport, EXSTATUS_INFORMATION, &infoStatus);
    }
    else memset(&infoStatus, 0, sizeof(infoStatus));

    /* Close the export on the document.  Required */
    seResult = EXCloseExport(hExport);

    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "EXCloseExport() failed: %s (0x%04X)\n", _szError, seResult);
        DACloseDocument(hDoc);
        DADeInit();
        goto ErrorExit;
    }
    else
    {
        if (Opts.dwOutputID == FI_MHTML)
            gdwFileCount = 1;
        else
            gdwFileCount++; /* EX_CALLBACK_ID_CREATENEWFILE not called for root file. */

        fprintf(stdout, "Export successful: %d output file(s) created.\n", gdwFileCount);

        /*
            Since we're closing down anyway, don't sweat errors, but only print
            the output on success.
        */
        if (SCCERR_OK == seStatusResult)
        {
            /*  Check status of export.  Only applicable to SearchML and XX.  */
            switch (Opts.dwOutputID)
            {
                case FI_SEARCHML_LATEST:
                case FI_SEARCHML30:
                case FI_SEARCHML31:
                case FI_SEARCHML32:
                case FI_SEARCHML33:
                case FI_SEARCHML34:
                case FI_SEARCHML35:
                case FI_SEARCHHTML:
                case FI_XML_FLEXIONDOC_LATEST:
                case FI_XML_FLEXIONDOC5_7:
                    fprintf(stdout, "%d subdocuments succeeded.\n", status.dwSucceeded);
                    fprintf(stdout, "%d subdocuments failed.\n", status.dwFailed);
            }

            if (Opts.dwExportStatusType == EXSTATUS_INFORMATION)
            {
                if (infoStatus.bMissingMap)
                    fprintf(stdout, "One or more PDF text runs was missing the toUnicode table.\n");

                if (infoStatus.bVerticalText)
                    fprintf(stdout, "One or more vertical text runs are present.\n");

                if (infoStatus.bTextEffects)
                    fprintf(stdout, "One or more unsupported text effects are present.\n");

                if (infoStatus.bUnsupportedCompression)
                    fprintf(stdout, "One or more unsupported compressions are present.\n");

                if (infoStatus.bUnsupportedColorSpace)
                    fprintf(stdout, "One or more unsupported color spaces are present.\n");

                if (infoStatus.bForms)
                    fprintf(stdout, "One or more forms are used.\n");

                if (infoStatus.bRightToLeftTables)
                    fprintf(stdout, "One or more tables containing rt-to-lft text are present.\n");

                if (infoStatus.bEquations)
                    fprintf(stdout, "One or more equations are present.\n");

                if (infoStatus.bAliasedFont)
                    fprintf(stdout, "One or more font aliases are used.\n");

                if (infoStatus.bMissingFont)
                    fprintf(stdout, "One or more fonts were not available on the system.\n");

                if (infoStatus.bTypeThreeFont)
                    fprintf(stdout, "One or more PDF files contained Type 3 embedded fonts.\n");

                if (infoStatus.bUnsupportedShading)
                    fprintf(stdout, "One or more unsupported shadings are present.\n");

                if (infoStatus.bInvalidHTML)
                    fprintf(stdout, "One or more files contained invalid HTML.\n");

                if (infoStatus.bVectorObjectLimit)
                    fprintf(stdout, "The vector object limit was reached and some vector objects were omitted.\n");

                if (infoStatus.bInvalidAnnotationNotApplied)
                    fprintf(stdout, "One or more annotations or redactions were invalid and were not rendered\n");

                if (infoStatus.bSubDocFailed)
                    fprintf(stdout, "One or more subdocuments failed.\n");

                if (infoStatus.bInlineImageFound)
                    fprintf(stdout, "One or more inline images were found and could not be rendered.\n");
				
                /* finally, if nothing is set, print out a message saying as much.*/
                if (!(infoStatus.bMissingMap ||
                      infoStatus.bVerticalText ||
                      infoStatus.bTextEffects ||
                      infoStatus.bUnsupportedCompression ||
                      infoStatus.bUnsupportedColorSpace ||
                      infoStatus.bForms ||
                      infoStatus.bRightToLeftTables ||
                      infoStatus.bEquations ||
                      infoStatus.bAliasedFont ||
                      infoStatus.bMissingFont ||
                      infoStatus.bSubDocFailed ||
                      infoStatus.bTypeThreeFont ||
                      infoStatus.bUnsupportedShading ||
                      infoStatus.bInvalidHTML ||
                      infoStatus.bVectorObjectLimit ||
                      infoStatus.bInvalidAnnotationNotApplied||
					  infoStatus.bInlineImageFound))
                {
                    fprintf(stdout, "No EXSTATUSINFORMATION flags were set.\n");
                }
            }
        }
    }

    /* Close the document.  Required */
    seResult = DACloseDocument(hDoc);
    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "DACloseDocument() failed: %s (0x%04X)\n", _szError, seResult);
        DADeInit();
        goto ErrorExit;
    }

    /* Shutdown the Data Access module.  Required */
    seResult = DADeInit();
    if (seResult != SCCERR_OK)
    {
        DAGetErrorString(seResult, _szError, sizeof(_szError));
        fprintf(stderr, "DADeInit() failed: %s (0x%04X)\n", _szError, seResult);
    }

ErrorExit:
    Cleanup(&Opts);

    return seResult;
}


SCCERR HandleEndPage(VTHEXPORT hExport, VTLPVOID pCommandData)
{
    VTLPDWORD    pPageNumber  = (VTLPDWORD)pCommandData;
    EXPROPERTIES exProperties = { 0 };
    SCCERR       err          = SCCERR_OK;

    if ((err = EXGetProperties(hExport, EX_PAGEPROP_ISHIDDEN | EX_PAGEPROP_SLIDENOTES, &exProperties)) == SCCERR_OK) 
    {
        if (exProperties.bIsHidden == TRUE)
            fprintf(stdout, "Finished page %ld (hidden = TRUE)\n", *pPageNumber);
        else
            fprintf(stdout, "Finished page %ld (hidden = FALSE)\n", *pPageNumber);
        
        if (exProperties.slideNotes.lpText && exProperties.slideNotes.dwTextLen > 0)
        {
            VTLPBYTE pSlideNoteText = NULL;
            VTDWORD i = 0;

            pSlideNoteText = (VTLPBYTE)malloc(sizeof(VTBYTE) * exProperties.slideNotes.dwTextLen+1);
            if (pSlideNoteText)
            {
                for (i = 0; i < exProperties.slideNotes.dwTextLen; ++i)
                {
                    pSlideNoteText[i] = (VTBYTE)exProperties.slideNotes.lpText[i];
                }
                pSlideNoteText[i] = 0x00;

                fprintf(stdout, "Text of slide notes: %s\n", (char *)pSlideNoteText);

                free(pSlideNoteText);
            }
        }
    }
    else
    {
        if (SCCERR_INVALIDPROPERTY == err)
            fprintf(stderr, "Invalid ID passed to EXGetProperties!\n");
        else if (!(SCCERR_INVALIDINPUTFORMAT == err)) 
            fprintf(stderr, "EXGetProperties failed with an unknown error.\n");
    }

    return err;
}

SCCERR ExportOemCallback(VTHEXPORT hExport, VTSYSPARAM dwCallbackData,
                         VTDWORD dwCommandID, VTLPVOID pCommandData)
{
    SCCERR seResult;
    EXGRAPHICEXPORTINFO *pGraphicExportInfo;
    
    UNUSED(dwCallbackData);

    /*
    |   This is a dummy callback routine just to show how they are
    |   coded.  It doesn't do anything terribly exciting.  All it does
    |   is keep a count of how many output files we created.
    |
    |   Note that XML Export does not use callbacks so this is not
    |   required or used.
    */
    seResult = SCCERR_NOTHANDLED; /* assume we don't handle the callback! */
    switch (dwCommandID)
    {
        case EX_CALLBACK_ID_CREATENEWFILE:
            gdwFileCount++;
            break;

        case EX_CALLBACK_ID_GRAPHICEXPORTFAILURE:
            pGraphicExportInfo = (EXGRAPHICEXPORTINFO *) pCommandData;
            if (pGraphicExportInfo->ExportGraphicStatus != SCCERR_OK)
            {
                DAGetErrorString(pGraphicExportInfo->ExportGraphicStatus, _szError, sizeof(_szError));
                fprintf(stderr, "\t Graphic failed: %s (%04X)\n", _szError, pGraphicExportInfo->ExportGraphicStatus);
                ++gdwGraphicExportErrors;
            }
            /*
            |   Note: If the OEM desires to substitute a graphic for the "Gray box" that is
            |   created by default when this callback routine returns SCCERR_NOTHANDLED,
            |   use *pGraphicExportInfo->hFile to write the substitute graphic file and
            |   then return SCCERR_OK.
            */
            break;

        case EX_CALLBACK_ID_ENTERARCHIVE:
            {
                EXENTERARCHIVECALLBACKDATA  *pEnterArchiveData = (EXENTERARCHIVECALLBACKDATA*)pCommandData;
                VTWORD i = 0;
                while(*(pEnterArchiveData->wzFullName) && i < 256)
                {
                    szFullName[i] = (VTBYTE)*(pEnterArchiveData->wzFullName);
                    i++;
                    pEnterArchiveData->wzFullName++;
                }
                gdwFileCount++;
            }
            break;
        case EX_CALLBACK_ID_LEAVEARCHIVE:
            {
                VTWORD i;
                EXLEAVEARCHIVECALLBACKDATA  *pLeaveArchiveData = (EXLEAVEARCHIVECALLBACKDATA*)pCommandData;
                DAGetErrorString(pLeaveArchiveData->ExportResult, _szError, sizeof(_szError));
                if (pLeaveArchiveData->ExportResult)
                {
                    fprintf(stderr, "\tEX_CALLBACK_ID_LEAVEARCHIVE:%s %s (%04X)\n", szFullName, _szError, pLeaveArchiveData->ExportResult);
                    ++gdwArchiveExportErrors;
                    gdwFileCount--;
                }
                for (i = 0; i < 256; i++)
                    szFullName[i] = 0;
            }
            break;
        case EX_CALLBACK_ID_OCRERROR:
            {
                SCCERR *err = (SCCERR *) pCommandData;
                fprintf(stderr, "\tEX_CALLBACK_ID_OCRERROR: (%04X)\n", *err);
            }
            break;

        case EX_CALLBACK_ID_PAGECOUNT:
            {
                VTLPDWORD pPageCount = (VTLPDWORD)pCommandData;

                if ( *pPageCount == 1 )
                    fprintf(stdout, "\tEX_CALLBACK_ID_PAGECOUNT: The File had %d page.\n", *pPageCount);
                else
                    fprintf(stdout, "\tEX_CALLBACK_ID_PAGECOUNT: The File had %d pages.\n", *pPageCount);
                
                seResult = SCCERR_OK;
            }
            break;

        case EX_CALLBACK_ID_ENDPAGE:
            {
                if (gbHandleEndPage)
                    seResult = HandleEndPage(hExport, pCommandData);
                else
                    seResult = SCCERR_NOTHANDLED;
            }
            break;

        case EX_CALLBACK_ID_BEGINPAGE:
        case EX_CALLBACK_ID_NEWFILEINFO:
        case EX_CALLBACK_ID_OEMOUTPUT:
        default:
            break;
    }
    return seResult;
}

