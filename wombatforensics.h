#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include <filesystem>
#include <byteswap.h>
#include <time.h>
#include <fcntl.h>
#include <thread>
// FOX TOOLKIT GUI 
#include "/usr/local/include/fox-1.7/fx.h"
// PROJECT INCLUDES
#include "icons.h"
#include "managetags.h"
#include "aboutbox.h"
#include "viewer.h"
#include "common.h"
#include "settings.h"
#include "managecarving.h"
#include "evidencemanager.h"
#include "forensicimage.h"
#include "partitions.h"
#include "directories.h"
#include "manageviewer.h"
#include "messagelog.h"
#include "filters.h"
#include "filterview.h"
#include "digdeeper.h"
#include "managehashlist.h"
//#include "hexviewer.h"
#include "artifactparser.h"
//#include "pdfviewer.h"
#include "propviewer.h"
//#include "imageviewer.h"
#include "thumbviewer.h"
// SHARED LIBRARIES
#include <tar.h> // TAR FOR WCF
#include <libtar.h> // TAR FOR WCF
//#define TICKS_PER_SECOND 10000000
//#define EPOCH_DIFFERENCE 11644473600LL
//#define NSEC_BTWN_1904_1970	(uint32_t) 2082844800U

//---------------------------------------------------------------
//
//  Derived checkbox table item
//
//---------------------------------------------------------------
class CheckTableItem : public FXTableItem
{
    FXDECLARE(CheckTableItem)

  protected:

    CheckTableItem() {}

  private:

    FXTable	    *Table;
    FXObject	    *Target;
    FXSelector	    Selector;

    FXCheckButton   *check;

    virtual void drawContent(const FXTable* table,FXDC& dc,FXint
x,FXint y,FXint w,FXint h) const;

  public:

    long onCheck(FXObject *, FXSelector, void *);

    enum {
	ID_CHECK = 1,
	ID_LAST
    };

  public:

    CheckTableItem(FXTable *table, FXIcon *ic=NULL, void *ptr=NULL, FXString str=FXString::null);
    virtual ~CheckTableItem(void) { delete check; }

    void setCheck(FXbool state=true, FXbool notify=false) {
check->setCheck(state, notify); }
    FXbool getCheck() const { return check->getCheck(); }
};


//---------------------------------------------------------------
//	CheckTableItem
//---------------------------------------------------------------
FXDEFMAP(CheckTableItem) CheckTableItemMap[] = {
  FXMAPFUNC(SEL_COMMAND, CheckTableItem::ID_CHECK, CheckTableItem::onCheck),
};

FXIMPLEMENT(CheckTableItem, FXTableItem, CheckTableItemMap,
ARRAYNUMBER(CheckTableItemMap))

// Construct new table item
CheckTableItem::CheckTableItem(FXTable *table, FXIcon *ic, void *ptr, FXString str):
    FXTableItem(str,ic,ptr)
{
    Table = table;
    Target = table->getTarget();
    Selector = table->getSelector();
    check = new FXCheckButton(table, str, this, ID_CHECK);
    check->setBackColor(table->getBackColor());
    check->create();
}

//
//	drawContent - override
//
void CheckTableItem::drawContent(const FXTable *table, FXDC &dc,
    FXint x, FXint y, FXint w, FXint h) const
{
    check->position(x+1,y+1,w-2,h-2);
    return;
}

//
//	onCheck
//
long CheckTableItem::onCheck(FXObject *, FXSelector, void *vp)
{
    if (Target) {
	FXTableRange tablerange;
	tablerange.fm.row = tablerange.to.row = Table->rowAtY(check->getY());
	tablerange.fm.col = tablerange.to.col = Table->colAtX(check->getX());
        //fxmessage("[%d,%d] = %p\n", tablerange.fm.row, tablerange.fm.col, vp);
	Target->handle(this, FXSEL(SEL_REPLACED,Selector), &tablerange);
    }
    return 1;
}

class WombatForensics : public FXMainWindow
{
    FXDECLARE(WombatForensics)

    private:
        FXVerticalFrame* mainframe;
        FXToolBar* toolbar;
        FXToolBar* pathtoolbar;
	//FXMenuBar* pathmenubar;
        FXSplitter* hsplitter;
        FXTable* tablelist;
        FXHeader* tableheader;
        FXStatusBar* statusbar;
        FXFont* plainfont;
        //FXText* plaintext;
	//FXImageView* imgview;

        FXIcon* newicon;
        FXButton* newbutton;
	FXIcon* openicon;
        FXButton* openbutton;
        FXIcon* saveicon;
        FXButton* savebutton;

	FXIcon* evidmanicon;
	FXButton* evidmanbutton;

        FXIcon* jumptohexicon;
        FXButton* jumptohexbutton;
        FXIcon* searchhexicon;
        FXButton* searchhexbutton;
        FXIcon* byteconvertericon;
        FXButton* byteconverterbutton;

        FXIcon* settingsicon;
        FXButton* settingsbutton;
        FXIcon* managecarvedicon;
        FXButton* managecarvedbutton;
        FXIcon* viewmanageicon;
        FXButton* viewmanagebutton;
        FXIcon* messagelogicon;
        FXButton* messagelogbutton;

        FXIcon* imagethumbicon;
        FXButton* imagethumbbutton;
        FXIcon* videothumbicon;
        FXButton* videothumbbutton;
        FXIcon* digdeepericon;
        FXButton* digdeeperbutton;
        FXIcon* carvingicon;
        FXButton* carvingbutton;
        FXIcon* exportfilesicon;
        FXButton* exportfilesbutton;
        FXIcon* exportwliicon;
        FXButton* exportwlibutton;

	FXIcon* managetagsicon;
	FXButton* managetagsbutton;
        FXIcon* managehashicon;
        FXButton* managehashbutton;
	FXIcon* previewicon;
	FXButton* previewbutton;
	FXIcon* publishicon;
	FXButton* publishbutton;

	/*
        FXIcon* createwfiicon;
        FXButton* createwfibutton;
        FXIcon* verifywfiicon;
        FXButton* verifywfibutton;
	*/

        FXIcon* xchompicon;
        FXButton* xchompbutton;
	FXIcon* abouticon;
	FXButton* aboutbutton;

        FXIcon* burrowicon;
        FXButton* burrowbutton;
	FXIcon* backicon;
	FXButton* backbutton;
	FXIcon* frwdicon;
	FXButton* frwdbutton;
	FXIcon* curicon;
	FXButton* curbutton;

        // ITEM TYPE ICONS
        FXIcon* forimgicon;
        FXIcon* carvedfileicon;
        FXIcon* defaultfileicon;
        FXIcon* defaultfoldericon;
        FXIcon* deletedfileicon;
        FXIcon* deletedfoldericon;
        FXIcon* partitionicon;
        FXIcon* virtualfileicon;
        FXIcon* virtualfoldericon;
        FXIcon* filtericon;

        std::vector<std::string> tags;
        std::vector<std::string> binaries;
        std::vector<std::string> whlfiles;
        FXArray<FXString> taggedlist;
        std::ifstream filebuffer;
	std::ofstream filewriter;
        FXArray<FXString> fileuserdata;
        FXString evidencelist;
        FXString curfileuserdata;
        FXString homepath;
        FXString tmppath;
        FXString casename;
        FXString configpath;
        FXString currentsettings;
	FXString currentcarvetypes;
	FXString prevevidpath;
        FXString itemtext;
        FXString pathtext;
        FXString filetext;
        FXString currentviewers;
        FXString pname;
        
        FXFile tagsfile;
        FXFile logfile;
        FXFile settingfile;
	FXFile carvetypesfile;
        FXFile evidencefile;
        FXFile binariesfile;
        
        char dtbuf[35];
        bool iscaseopen;
	bool isfrompath;
        
        Viewer* viewer;
        ForImg* curforimg;
        std::vector<ForImg*> forimgvector;
        MessageLog* msglog;
	ThumbViewer* imagethumbviewer;
        ThumbViewer* videothumbviewer;

        uint64_t globalid;
        uint64_t curid;
        uint64_t lastid;

	CurrentItem currentitem;
        FileItem currentfileitem;
        
        int sortindex = 1;
        uint sortasc = 1;
        int itemtype = 0;
        int curiconid = 0;
        std::vector<std::string> volnames;
        std::vector<uint64_t> volsizes;
        std::vector<uint64_t> voloffsets;
        std::vector<FileItem> fileitemvector;
	std::vector<FXString> vidthumbpathlist;
	std::vector<FXString> imgthumbpathlist;

    protected:
        WombatForensics() {}
	~WombatForensics();

    public:
        enum
        {
            ID_TREELIST = 1,
            ID_NEW = 99,
            ID_OPEN = 100,
            ID_SAVE = 98,
	    ID_EVIDMANAGE = 97,
            ID_SETTINGS = 95,
            ID_VIEWMANAGE = 94,
            ID_MSGLOG = 93,
            ID_JUMPHEX = 92,
            ID_SEARCHHEX = 91,
            ID_BYTECONV = 90,
            ID_IMGVIDTHUMB = 89,
            ID_DIGDEEPER = 88,
            ID_CARVING = 87,
            ID_EXPORTFILES = 86,
            ID_EXPORTWLI = 85,
            ID_MANAGEHASH = 84,
            ID_CREATEWFI = 83,
            ID_VERIFYWFI = 82,
            ID_XCHOMP = 81,
            ID_MANAGECARVED = 80,
            ID_TREESELECT = 101,
	    ID_MANAGETAGS = 102,
	    ID_PREVIEW = 103,
	    ID_PUBLISH = 104,
	    ID_ABOUT = 105,
	    ID_TABLESELECT = 106,
            ID_TAGMENU = 107,
            ID_NEWTAG = 108,
            ID_SETTAG = 109,
            ID_REMTAG = 110,
            ID_PAGESPIN = 111,
            ID_SQLLIST = 112,
            ID_PROPTABLE = 113,
            ID_OFFTEXT = 114,
            ID_HEXTEXT = 115,
            ID_ASCTEXT = 116,
            ID_SCROLLBAR = 117,
            ID_HOME = 118,
	    ID_PARTITION = 119,
	    ID_BACK = 120,
	    ID_FRWD = 121,
	    ID_CURRENT = 122,
            ID_TABLEHEADER = 123,
            ID_BINARY = 124,
            ID_CONTENTS = 125,
            ID_PROPERTIES = 126,
            ID_CHECKIT = 127,
            ID_UNCHECKALL = 128,
            ID_HEXCONTENTS = 129,
            ID_IMAGETHUMB = 130,
            ID_VIDEOTHUMB = 131,
            ID_LAST
        };
        WombatForensics(FXApp* a);
        long NewCase(FXObject*, FXSelector, void*);
        long OpenCase(FXObject*, FXSelector, void*);
        long SaveCase(FXObject*, FXSelector, void*);
	long ManageEvidence(FXObject*, FXSelector, void*);
        long OpenTagManager(FXObject*, FXSelector, void*);
	long OpenAboutBox(FXObject*, FXSelector, void*);
        long PreviewReport(FXObject*, FXSelector, void*);
        long TagMenu(FXObject*, FXSelector, void*);
        long TableUpDown(FXObject*, FXSelector, void*);
        long ContentSelected(FXObject*, FXSelector, void*);
        long LoadForensicImages(FXObject*, FXSelector, void*);
        long LoadChildren(FXObject*, FXSelector, void*);
        long LoadCurrent(FXObject*, FXSelector, void*);
        long LoadCurrentPath(FXObject*, FXSelector, void*);
        long LoadCurrentFile(FXObject*, FXSelector, void*);
        long OpenSettings(FXObject*, FXSelector, void*);
        long OpenManageCarved(FXObject*, FXSelector, void*);
        long OpenViewerManager(FXObject*, FXSelector, void*);
        long OpenMessageLog(FXObject*, FXSelector, void*);
	long OpenDigDeeper(FXObject*, FXSelector, void*);
	long OpenManageHashList(FXObject*, FXSelector, void*);
        long OpenXChomp(FXObject*, FXSelector, void*);
        long SortColumn(FXObject* sender, FXSelector sel, void* colid);
        long FilterColumn(FXObject* sender, FXSelector sel, void* colid);
        long CheckSelected(FXObject* sender, FXSelector sel, void*);
        long OpenHexViewer(FXObject*, FXSelector, void*);
	long OpenPropertyViewer(FXObject*, FXSelector, void*);
	//long OpenThumbViewer(FXObject*, FXSelector, void*);
	long OpenImageThumbViewer(FXObject*, FXSelector, void*);
	long OpenVideoThumbViewer(FXObject*, FXSelector, void*);
        /*
        long KeySelected(FXObject*, FXSelector, void*);
	long ValueSelected(FXObject*, FXSelector, void*);
        long SetTag(FXObject* sender, FXSelector, void*);
        long CreateNewTag(FXObject*, FXSelector, void*);
        long RemoveTag(FXObject*, FXSelector, void*);
        long PublishReport(FXObject*, FXSelector, void*);
        long FileSelected(FXObject*, FXSelector, void*);
        long PropertySelected(FXObject*, FXSelector, void*);
        long TableUp(FXObject*, FXSelector, void*);
        long ContentSelected(FXObject*, FXSelector, void*);
        long TableUpDown(FXObject*, FXSelector, void*);
        long PageChanged(FXObject*, FXSelector, void*);
        long ScrollChanged(FXObject*, FXSelector, void*);
	//void PopulateChildKeys(libregf_key_t* curkey, FXTreeItem* curitem, libregf_error_t* regerr);
	void GetRootString(FXTreeItem* curitem, FXString* rootstring);
	FXString ConvertWindowsTimeToUnixTimeUTC(uint64_t input);
        FXString ConvertUnixTimeToString(uint32_t input);
        FXString DecryptRot13(FXString encstr);
        FXchar Rot13Char(FXchar curchar);
        uint GetVarIntLength(uint8_t* pagearray, uint64_t pageoffset);
        uint GetVarInt(uint8_t* pagearray, uint64_t pageoffset, uint varintlength);
        uint64_t GetSerialType(uint64_t contentoffset, FXString* tmptype, FXString* tmpval, FXString* tmpofflen, uint8_t* pagearray, int serialtype);
        void LoadPage(void);
        void PopulatePageContent(uint8_t* pagearray);
        void ParseFileHeader(uint8_t* pageheader);
        void PopulateFileHeader(void);
        void ParsePageHeader(uint8_t* pagearray, uint8_t fileheader, uint64_t curpage);
        void PopulatePageHeader(FXArray<uint16_t>* celloffsetarray);
        void ParseRowContents(uint8_t* pagearray, FXArray<uint16_t>* celloffsetarray);
        void PopulatePropertyTable(FXArray<uint16_t>* celloffsetarray);
        void AddProperty(int row, FXString offlen, FXString val, FXString desc);
        void AddContent(int row, FXString islive, FXString rowid, FXString offlen, FXString type, FXString val, FXString tag);
        */
        void AlignColumn(FXTable* curtable, int row, FXuint justify);
        void FitColumnContents(int col);
        FXString GetSettings(int setting);
	void SaveCurrentCase(void);
        void CloseCurrentCase(void);
        void DeleteTmpFiles(void);
        void EnableCaseButtons(void);
        void LoadCaseState(void);
        void IncrementGlobalId(uint64_t* globalid, uint64_t* curid);
	void SortFileTable(std::vector<FileItem>* fileitems, FXString filestr, FXint filecount, int itemindex=1, int asc=1);
        void PlainView(FileItem* curfileitem);
        //void ApplyFilter(int colindex, std::string filterstring);
        //void LoadPartitions(ForImg* curforimg);
        //void LoadGptPartitions(ForImg* curforimg);
        //FXString GetFileSystemName(ForImg* curforimg, uint64_t offset);
	//void GetNextCluster(ForImg* curimg, uint32_t clusternum, uint8_t fstype, uint64_t fatoffset, FXArray<uint>* clusterlist);
	//FXString ConvertBlocksToExtents(FXArray<uint> blocklist, uint blocksize, uint64_t rootdiroffset);
	void UpdateForensicImages(void);
        void InitializeThumbCache(void);

        void LogEntry(FXString logstring)
        {
            FXString tmpstr = FXString(GetDateTime(dtbuf)) + " | " + logstring + "\n";
            FXFile* logfile = new FXFile(tmppath + "msglog", FXIO::ReadWrite|FXIO::Append, FXIO::OwnerReadWrite);
            int wrerr = logfile->writeBlock(tmpstr.text(), tmpstr.length());
            logfile->close();
        };
	void StatusUpdate(FXString tmptext)
	{
	    statusbar->getStatusLine()->setNormalText(tmptext);
	};
        virtual void create();

};

FXDEFMAP(WombatForensics) WombatForensicsMap[]={
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_NEW, WombatForensics::NewCase),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_OPEN, WombatForensics::OpenCase),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_SAVE, WombatForensics::SaveCase),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_EVIDMANAGE, WombatForensics::ManageEvidence),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_ABOUT, WombatForensics::OpenAboutBox),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_MANAGETAGS, WombatForensics::OpenTagManager),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_PREVIEW, WombatForensics::PreviewReport),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_SETTINGS, WombatForensics::OpenSettings),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_MANAGECARVED, WombatForensics::OpenManageCarved),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_DIGDEEPER, WombatForensics::OpenDigDeeper),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_MANAGEHASH, WombatForensics::OpenManageHashList),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_HOME, WombatForensics::LoadForensicImages),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_CURRENT, WombatForensics::LoadCurrent),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_BACK, WombatForensics::LoadCurrentPath),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_FRWD, WombatForensics::LoadCurrentFile),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_VIEWMANAGE, WombatForensics::OpenViewerManager),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_MSGLOG, WombatForensics::OpenMessageLog),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_XCHOMP, WombatForensics::OpenXChomp),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, WombatForensics::ID_TABLESELECT, WombatForensics::TagMenu),
    FXMAPFUNC(SEL_KEYPRESS, WombatForensics::ID_TABLESELECT, WombatForensics::TableUpDown),
    FXMAPFUNC(SEL_SELECTED, WombatForensics::ID_TABLESELECT, WombatForensics::ContentSelected),
    FXMAPFUNC(SEL_DOUBLECLICKED, WombatForensics::ID_TABLESELECT, WombatForensics::LoadChildren),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_TABLEHEADER, WombatForensics::SortColumn),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_CHECKIT, WombatForensics::CheckSelected),
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, WombatForensics::ID_TABLEHEADER, WombatForensics::FilterColumn),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_HEXCONTENTS, WombatForensics::OpenHexViewer),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_PROPERTIES, WombatForensics::OpenPropertyViewer),
    //FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_IMGVIDTHUMB, WombatForensics::OpenThumbViewer),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_IMAGETHUMB, WombatForensics::OpenImageThumbViewer),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_VIDEOTHUMB, WombatForensics::OpenVideoThumbViewer),

    /*
    //FXMAPFUNC(SEL_CLICKED, WombatForensics::ID_TREESELECT, WombatForensics::KeySelected),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_OPEN, WombatForensics::OpenSqliteFile),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_MANAGETAGS, WombatForensics::OpenTagManager),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_ABOUT, WombatForensics::OpenAboutBox),
    //FXMAPFUNC(SEL_SELECTED, WombatForensics::ID_TABLESELECT, WombatForensics::ValueSelected),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_NEWTAG, WombatForensics::CreateNewTag),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_SETTAG, WombatForensics::SetTag),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_REMTAG, WombatForensics::RemoveTag),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_PUBLISH, WombatForensics::PublishReport),
    FXMAPFUNC(SEL_SELECTED, WombatForensics::ID_SQLLIST, WombatForensics::FileSelected),
    FXMAPFUNC(SEL_SELECTED, WombatForensics::ID_PROPTABLE, WombatForensics::PropertySelected),
    FXMAPFUNC(SEL_KEYPRESS, WombatForensics::ID_PROPTABLE, WombatForensics::TableUp),
    FXMAPFUNC(SEL_KEYPRESS, WombatForensics::ID_TABLESELECT, WombatForensics::TableUpDown),
    FXMAPFUNC(SEL_SELECTED, WombatForensics::ID_TABLESELECT, WombatForensics::ContentSelected),
    FXMAPFUNC(SEL_COMMAND, WombatForensics::ID_PAGESPIN, WombatForensics::PageChanged),
    FXMAPFUNC(SEL_CHANGED, WombatForensics::ID_SCROLLBAR, WombatForensics::ScrollChanged),
    */
};
#endif // WOMBATSQLITE_H
