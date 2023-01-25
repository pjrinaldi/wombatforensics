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
    FXArray<FXint> posarray;
    posarray.clear();
    int found = 0;
    posarray.append(-1);
    //std::cout << cursettings.text() << std::endl;
    while(found > -1)
    {
        found = cursettings.find('\n', found+1);
        if(found > -1)
            posarray.append(found);
    }
    for(int i=0; i < posarray.no() - 1; i++)
    {
        FXString curstring = cursettings.mid(posarray.at(i)+1, posarray.at(i+1) - posarray.at(i));
        int found1 = curstring.find(",");
        int found2 = curstring.find(",", found1+1);
        int found3 = curstring.find(",", found2+1);
        int found4 = curstring.find(",", found3+1);
        int found5 = curstring.find(",", found4+1);
        filetypetable->setItemText(i, 0, curstring.left(found1));
        filetypetable->setItemText(i, 1, curstring.mid(found1+1, found2 - found1 - 1));
        filetypetable->setItemText(i, 2, curstring.mid(found2+1, found3 - found2 - 1));
        filetypetable->setItemText(i, 3, curstring.mid(found3+1, found4 - found3 - 1));
        filetypetable->setItemText(i, 4, curstring.mid(found4+1, found5 - found4 - 1));
        filetypetable->setItemText(i, 5, curstring.mid(found5+1, curstring.length() - found5 -3));
    }
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
