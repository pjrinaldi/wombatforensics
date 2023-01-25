#include "managecarving.h"

FXIMPLEMENT(ManageCarving,FXDialogBox,ManageCarvingMap,ARRAYNUMBER(ManageCarvingMap))

ManageCarving::ManageCarving(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 680, 400, 0,0,0,0, 10, 10)
{
    //plainfont = new FXFont(this->getApp(), "monospace");

    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe1, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe1);
    addrowbutton = new FXButton(hframe1, "Add Row", NULL, this, ID_ADDROW, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    savebutton = new FXButton(hframe1, "Save", NULL, this, FXDialogBox::ID_ACCEPT,FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    closebutton = new FXButton(hframe1, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    //filetypetext = new FXText(mainframe, this, ID_FTTEXT, LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    //filetypetext->setFont(plainfont);
    filetypetable = new FXTable(mainframe, this, ID_FTTABLE, TABLE_COL_SIZABLE|LAYOUT_LEFT|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    filetypetable->setTableSize(10, 6);
    filetypetable->setColumnText(0, "Category");
    filetypetable->setColumnText(1, "Description");
    filetypetable->setColumnText(2, "Header");
    filetypetable->setColumnText(3, "Footer");
    filetypetable->setColumnText(4, "Extension");
    filetypetable->setColumnText(5, "Maximum File Size");
    filetypetable->setRowHeaderWidth(0);
    filetypetable->setColumnHeaderHeight(filetypetable->getColumnHeaderHeight() + 5);
}

long ManageCarving::AddRow(FXObject*, FXSelector, void*)
{
    filetypetable->insertRows(filetypetable->getNumRows(), 1);

    return 1;
}

FXString ManageCarving::ReturnManageCarving()
{
    /*
    FXString settingsstring = FXString::value(thumbsizespinner->getValue());
    settingsstring += "|" + FXString::value(vidthumbspinner->getValue());
    settingsstring += "|" + casepathtextfield->getText();
    settingsstring += "|" + reportpathtextfield->getText();
    settingsstring += "|" + reporttzcombo->getItemText(reporttzcombo->getCurrentItem());
    settingsstring += "|" + FXString::value(autosavespinner->getValue());
    return settingsstring;
    */
    return "";
}

void ManageCarving::LoadManageCarving(FXString cursettings)
{
    /*
    int i = 0;
    FXArray<FXString> filetypearray;
    filetypearray.clear();
    while(i < cursettings.length())
    {
        int found1 = cursettings.find("\n", i);
        int found2 = cursettings.find("\n", found1+1);
        filetypearray.append(cursettings.mid(found1+1, found2 - found1 - 1));
        i = found2 + 1;
        std::cout << "filetypearray: " << cursettings.mid(found1+1, found2 - found1 - 1).text() << std::endl;
    }
    */
    /*
    int found1 = cursettings.find("|");
    int found2 = cursettings.find("|", found1+1);
    int found3 = cursettings.find("|", found2+1);
    int found4 = cursettings.find("|", found3+1);
    int found5 = cursettings.find("|", found4+1);
    thumbsizespinner->setValue(cursettings.left(found1).toUInt());
    vidthumbspinner->setValue(cursettings.mid(found1+1, found2 - found1 - 1).toUInt());
    casepathtextfield->setText(cursettings.mid(found2+1, found3 - found2 - 1));
    reportpathtextfield->setText(cursettings.mid(found3+1, found4 - found3 - 1));
    reporttzcombo->setCurrentItem(cursettings.mid(found4+1, found5 - found4 - 1).toUInt());
    autosavespinner->setValue(cursettings.mid(found5+1, cursettings.length() - found5 - 1).toUInt());
    */
}

long ManageCarving::SetCasePath(FXObject*, FXSelector, void*)
{
    /*
    FXString casepathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store Wombat Case Files", casepathtextfield->getText());
    if(!casepathstring.empty())
        casepathtextfield->setText(casepathstring + "/");
    */

    return 1;
}

long ManageCarving::SetReportPath(FXObject*, FXSelector, void*)
{
    /*
    FXString reportpathstring = FXDirDialog::getOpenDirectory(this, "Select the Directory to store the Reports", reportpathtextfield->getText());
    if(!reportpathstring.empty())
        reportpathtextfield->setText(reportpathstring + "/");
    */

    return 1;
}

long ManageCarving::OpenCarvingManager(FXObject*, FXSelector, void*)
{
    //std::cout << "open carving manager dialog here." << std::endl;

    return 1;
}
