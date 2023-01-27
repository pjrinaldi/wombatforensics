#ifndef WOMBATFORENSICS_H
#define WOMBATFORENSICS_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <filesystem>
#include <byteswap.h>
#include <time.h>
#include <fcntl.h>
#include "/usr/local/include/fox-1.7/fx.h"
#include "icons.h"
#include "managetags.h"
#include "aboutbox.h"
#include "viewer.h"
#include "common.h"
#include "settings.h"
#include "managecarving.h"
#include "evidencemanager.h"
#include "forensicimage.h"
// SHARED LIBRARIES
#include <tar.h>
#include <libtar.h>

#define TICKS_PER_SECOND 10000000
#define EPOCH_DIFFERENCE 11644473600LL
#define NSEC_BTWN_1904_1970	(uint32_t) 2082844800U

// may not need this structure, if i just in time load the table items.
struct EvidenceItem
{
    int id;
    FXString fullpath;
    FXString evidencename; // may not need these
    FXString evidencepath; // may not need these
};

/*
struct WalHeader
{
    uint32_t header; // = qFromBigEndian<uint32_t>(pageheader->mid(0, 4));
    uint32_t fileversion; // = qFromBigEndian<uint32_t>(pageheader->mid(4, 4));
    uint32_t pagesize; // = qFromBigEndian<uint32_t>(pageheader->mid(8, 4));
    uint32_t checkptseqnum; // = qFromBigEndian<uint32_t>(pageheader->mid(12, 4));
    uint32_t salt1; // = qFromBigEndian<uint32_t>(pageheader->mid(16, 4));
    uint32_t salt2; // = qFromBigEndian<uint32_t>(pageheader->mid(20, 4));
    uint32_t checksum1; // = qFromBigEndian<uint32_t>(pageheader->mid(24, 4));
    uint32_t checksum2; // = qFromBigEndian<uint32_t>(pageheader->mid(28, 4));
};

struct JournalHeader
{
    uint64_t header; //= qFromBigEndian<uint64_t>(pageheader->mid(0, 8));
    uint32_t pagecnt; // = qFromBigEndian<uint32_t>(pageheader->mid(8, 4));
    uint32_t randomnonce; // = qFromBigEndian<uint32_t>(pageheader->mid(12, 4));
    uint32_t initsize; // = qFromBigEndian<uint32_t>(pageheader->mid(16, 4));
    uint32_t sectorsize; // = qFromBigEndian<uint32_t>(pageheader->mid(20, 4));
    uint32_t pagesize; // = qFromBigEndian<uint32_t>(pageheader->mid(24, 4));
};

struct SqliteHeader
{
    FXString header; // = FXString::fromStdString(pageheader->mid(0, 16).toStdString());
    uint16_t pagesize; // = qFromBigEndian<uint16_t>(pageheader->mid(16, 2));
    uint8_t writeversion; // = qFromBigEndian<uint8_t>(pageheader->mid(18, 1));
    uint8_t readversion; // = qFromBigEndian<uint8_t>(pageheader->mid(19, 1));
    uint8_t unusedpagespace; // = qFromBigEndian<uint8_t>(pageheader->mid(20, 1));
    uint32_t pagecount; // = qFromBigEndian<uint32_t>(pageheader->mid(28, 4));
    uint32_t firstfreepagenum; // = qFromBigEndian<uint32_t>(pageheader->mid(32, 4));
    uint32_t freepagescount; // 36, 4
    uint32_t schemacookie; // 40, 4
    uint32_t schemaformat; // 44, 4 - 1,2,3,4
    uint32_t pagecachesize; // 48, 4
    uint32_t largestrootbtreepagenumber; // 52, 4 - or zero
    uint32_t textencoding; // 56, 4 - 1 utf-8, 2 utf-16le, 3 utf-16be
    uint32_t userversion; // 60, 4
    uint32_t incrementalvacuummodebool; // 64, 4 - 0 = false, otherwise true
    uint32_t appid; // 68, 4
    char reserved[20]; // 72, 20
    uint32_t versionvalidfornum; // 92, 4
    uint32_t version; // 96, 4
};

struct PageHeader
{
    uint8_t type; // page type 0x02 - index interior (12) | 0x05 - table interior (12) | 0x0a - index leaf (8) | 0x0d - table leaf (8) [0,1]
    uint16_t firstfreeblock; // start of first free block on hte page or zero for no free blocks [1, 2]
    uint16_t cellcount; // number of cells on the page [3, 2]
    uint16_t cellcontentstart; // start of cell content area, zero represents 65536 [5, 2]
    uint8_t fragmentedfreebytescount; // number of fragmented free bytes within cell content area [7, 1]
    uint32_t rightmostpagenumber; // largest page number, right most pointer, interior page only [8, 4]
};

struct FrameHeader
{
    uint32_t pagenumber; // pagenumber
    uint32_t pagecount; // size of db file in pages for commits or zero
    uint32_t salt1; // salt-1 from the wal header
    uint32_t salt2; // salt-2 from the wal header
    uint32_t checksum1; // checksum-1 cumulative checksum up through and including this page
    uint32_t checksum2; // checksum-2 second half of cumulative checksum
};
*/
/*
class FXAPI FXComboTableItem : public FXTableItem {
FXDECLARE(FXComboTableItem)
protected:
    FXString selections;
private:
    FXComboTableItem(const FXComboTableItem&);
    FXComboTableItem& operator=(const FXComboTableItem&);
protected:
    FXComboTableItem(){}
public:
    FXComboTableItem(const FXString& text,FXIcon* ic=nullptr,void* ptr=nullptr);
    virtual FXWindow *getControlFor(FXTable* table);
    virtual void setFromControl(FXWindow *control);
    void setSelections(const FXString& strings);
    const FXString& getSelections() const { return selections; }
};
 */ 

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
        FXSplitter* hsplitter;
        FXTable* tablelist;
        FXStatusBar* statusbar;
        FXFont* plainfont;
        FXText* plaintext;

        FXIcon* newicon;
        FXButton* newbutton;
	FXIcon* openicon;
        FXButton* openbutton;
        FXIcon* saveicon;
        FXButton* savebutton;

	FXIcon* evidmanicon;
	FXButton* evidmanbutton;
	/*
        FXIcon* listaddicon;
        FXButton* listaddbutton;
        FXIcon* listremicon;
        FXButton* listrembutton;
	*/

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

        FXIcon* imgvidthumbicon;
        FXButton* imgvidthumbbutton;
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

        //FXArray<FXString> sqlitefiles;
        std::vector<std::string> tags;
        FXArray<FXString> taggedlist;
        std::ifstream filebuffer;
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
        
        FXFile tagsfile;
        FXFile logfile;
        FXFile settingfile;
	FXFile carvetypesfile;
        FXFile evidencefile;
        
        char dtbuf[35];
        bool iscaseopen;
        
        Viewer* viewer;
        ForImg* forimg;
        std::vector<ForImg*> forimgvector;


        /*
        uint8_t filetype = 0;
        uint64_t filesize = 0;
        uint64_t pagecount = 0;
        uint32_t pagesize = 0;
        uint64_t curpage = 0;
        int proptablerowcnt = 0;
        JournalHeader journalheader;
        WalHeader walheader;
        SqliteHeader sqliteheader;
        PageHeader pageheader;
        FrameHeader frameheader;
        */

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
            //ID_ADDEVID = 97,
            //ID_REMEVID = 96,
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
            ID_LAST
        };
        WombatForensics(FXApp* a);
        /*
        long OpenSqliteFile(FXObject*, FXSelector, void*);
        */
        long NewCase(FXObject*, FXSelector, void*);
        long OpenCase(FXObject*, FXSelector, void*);
        long SaveCase(FXObject*, FXSelector, void*);
	long ManageEvidence(FXObject*, FXSelector, void*);
        long OpenTagManager(FXObject*, FXSelector, void*);
	long OpenAboutBox(FXObject*, FXSelector, void*);
        long PreviewReport(FXObject*, FXSelector, void*);
        long TagMenu(FXObject*, FXSelector, void*);
        long OpenSettings(FXObject*, FXSelector, void*);
        long OpenManageCarved(FXObject*, FXSelector, void*);
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
        void AlignColumn(FXTable* curtable, int row, FXuint justify);
        */
        FXString GetSettings(int setting);
	void SaveCurrentCase(void);
        void CloseCurrentCase(void);
        void EnableCaseButtons(void);
        void LoadCaseState(void);

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
    FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, WombatForensics::ID_TABLESELECT, WombatForensics::TagMenu),
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
