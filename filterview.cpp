#include "filterview.h"

FXIMPLEMENT(FilterView,FXDialogBox,FilterViewMap,ARRAYNUMBER(FilterViewMap))

FilterView::FilterView(FXWindow* parent, const FXString& title):FXDialogBox(parent, title, DECOR_TITLE|DECOR_RESIZE|DECOR_BORDER|DECOR_CLOSE, 0, 0, 680, 400, 0,0,0,0, 10, 10)
{
    mainframe = new FXVerticalFrame(this, LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    filteredlist = new FXTable(mainframe, this, ID_FTABLE, TABLE_COL_SIZABLE|LAYOUT_TOP|LAYOUT_FILL_X|LAYOUT_FILL_Y);
    filteredlist->setTableSize(5,14);
    filteredlist->setRowHeaderWidth(0);
    filteredlist->setColumnText(1, "ID");
    filteredlist->setColumnHeaderHeight(filteredlist->getColumnHeaderHeight() + 5);
    filtericon = new FXPNGIcon(this->getApp(), filter);
    filtericon->create();
    /*
    hframe1 = new FXHorizontalFrame(mainframe, LAYOUT_TOP|LAYOUT_FILL_X);
    new FXLabel(hframe1, "", NULL, LAYOUT_FILL_X);
    new FXSpring(hframe1);
    addrowbutton = new FXButton(hframe1, "Add Row", NULL, this, ID_ADDROW, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    savebutton = new FXButton(hframe1, "Save", NULL, this, FXDialogBox::ID_ACCEPT,FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
    closebutton = new FXButton(hframe1, "Cancel", NULL, this, FXDialogBox::ID_CANCEL, FRAME_RAISED|FRAME_THICK, 0,0,0,0, 20,20);
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
    */
}

FilterView::~FilterView()
{
    filtericon->destroy();
}

void FilterView::FitColumnContents(int col)
{
    filteredlist->fitColumnsToContents(col);
    filteredlist->setColumnWidth(col, filteredlist->getColumnWidth(col) + 10);
}

void FilterView::AlignColumn(FXTable* curtable, int col, FXuint justify)
{
    for(int i=0; i < curtable->getNumRows(); i++)
        curtable->setItemJustify(i, col, justify);
}

void FilterView::ApplyFilter(CurrentItem* currentitem, int colindex, std::string filterstring)
{
    std::cout << "colindex: " << colindex << " filterstring: " << filterstring << std::endl;

    std::vector<std::string> filearray;
    filearray.clear();
    std::string pathstr = currentitem->tmppath + "burrow/";
    std::string filefilestr = currentitem->forimg->ImageFileName() + "." + std::to_string(currentitem->voloffset);
    if(colindex == 1) // id
    {
        filefilestr += "." + filterstring;
    }
    //std::cout << "file to match: " << filefilestr << std::endl;
    for(const auto & entry : std::filesystem::directory_iterator(pathstr))
    {
        //std::cout << "entry stem: " << entry.path().filename() << std::endl;
        if(filefilestr.compare(entry.path().filename()) == 0)
        {
            filearray.push_back(entry.path().string());
            //std::cout << "child file match: " << entry.path().string() << std::endl;
        }
    }
    // table initialization
    filteredlist->setTableSize(filearray.size(), 14);
    filteredlist->setColumnText(0, "ROW");
    filteredlist->setColumnText(1, "ID");
    filteredlist->setColumnText(2, "Name");
    filteredlist->setColumnText(3, "Path");
    filteredlist->setColumnText(4, "Size (bytes)");
    filteredlist->setColumnText(5, "Created (UTC)");
    filteredlist->setColumnText(6, "Accessed (UTC)");
    filteredlist->setColumnText(7, "Modified (UTC)");
    filteredlist->setColumnText(8, "Changed (UTC)");
    filteredlist->setColumnText(9, "Hash");
    filteredlist->setColumnText(10, "Category");
    filteredlist->setColumnText(11, "Signature");
    filteredlist->setColumnText(12, "Tagged");
    filteredlist->setColumnText(13, "Hash Match");
    std::cout << "filearray count: " << filearray.size() << std::endl;
    for(int i=0; i < filearray.size(); i++)
    {
        std::ifstream filestream;
        filestream.open(filearray.at(i).c_str(), std::ios::in | std::ios::ate);
        auto readsize = filestream.tellg();
        std::string filecontent(readsize, '\0');
        filestream.seekg(0);
        filestream.read(&filecontent[0], readsize);
        filestream.close();
        filteredlist->setItemText(i, 0, FXString::value(i+1));
        filteredlist->setItemText(i, 1, GetFileItem(&filecontent, 0).c_str());
        filteredlist->setItemText(i, 2, GetFileItem(&filecontent, 4).c_str());
        filteredlist->setItemText(i, 3, GetFileItem(&filecontent, 5).c_str());
        filteredlist->setItemText(i, 4, ReturnFormattingSize(std::stoull(GetFileItem(&filecontent, 3).c_str())).c_str());
        filteredlist->setItemText(i, 5, GetFileItem(&filecontent, 6).c_str());
        filteredlist->setItemText(i, 6, GetFileItem(&filecontent, 7).c_str());
        filteredlist->setItemText(i, 7, GetFileItem(&filecontent, 8).c_str());
        //filteredlist->setItemText(i, 8, GetFileItem(&filecontent, 16).c_str());
        //filteredlist->setItemText(i, 9, GetFileItem(&filecontent, 11).c_str());
        //filteredlist->setItemText(i, 10, GetFileItem(&filecontent, 12).c_str());
        //filteredlist->setItemText(i, 11, GetFileItem(&filecontent, 13).c_str());
        //filteredlist->setItemText(i, 12, GetFileItem(&filecontent, 14).c_str());
        //filteredlist->setItemText(i, 13, GetFileItem(&filecontent, 15).c_str());
    }
    // table formatting
    filteredlist->fitColumnsToContents(0);
    filteredlist->setColumnWidth(0, filteredlist->getColumnWidth(0) + 25);
    FitColumnContents(1);
    FitColumnContents(2);
    FitColumnContents(4);
    AlignColumn(filteredlist, 1, FXTableItem::LEFT);
    AlignColumn(filteredlist, 2, FXTableItem::LEFT);
    AlignColumn(filteredlist, 3, FXTableItem::LEFT);
    AlignColumn(filteredlist, 4, FXTableItem::LEFT);
    AlignColumn(filteredlist, 5, FXTableItem::LEFT);
    AlignColumn(filteredlist, 6, FXTableItem::LEFT);
    AlignColumn(filteredlist, 7, FXTableItem::LEFT);
    
    /*
            //std::cout << "filestr " << i << ": " << filearray.at(i) << std::endl;
            FileItem tmpitem;
            tmpitem.gid = std::stoull(GetFileItem(&filecontent, 0).c_str());
            tmpitem.isdeleted = std::stoi(GetFileItem(&filecontent, 1).c_str());
            tmpitem.isdirectory = std::stoi(GetFileItem(&filecontent, 2).c_str());
            tmpitem.size = std::stoull(GetFileItem(&filecontent, 3).c_str());
            tmpitem.name = GetFileItem(&filecontent, 4);
            filteredlist->setItemText(i, 4, FXString(ReturnFormattingSize(fileitems->at(i).size).c_str()));
            //std::cout << "read file name: " << tmpitem.name << " isdirectory: " << tmpitem.isdirectory << std::endl;
            tmpitem.path = GetFileItem(&filecontent, 5);
            tmpitem.create = GetFileItem(&filecontent, 6);
            tmpitem.access = GetFileItem(&filecontent, 7);
            tmpitem.modify = GetFileItem(&filecontent, 8);
            tmpitem.layout = GetFileItem(&filecontent, 9);
            tmpitem.isvirtual = std::stoi(GetFileItem(&filecontent, 10).c_str());
            tmpitem.hash = GetFileItem(&filecontent, 11);
            tmpitem.cat = GetFileItem(&filecontent, 12);
            tmpitem.sig = GetFileItem(&filecontent, 13);
            tmpitem.tag = GetFileItem(&filecontent, 14);
            tmpitem.match = GetFileItem(&filecontent, 15);
            //std::cout << "File item Values: " << std::endl;
            //std::cout << "filecontent:" << filecontent << std::endl;
            //std::cout << tmpid << " " << tmpitem.isdeleted << " " << tmpitem.isdirectory << std::endl;
            //std::cout << tmpitem.size << " " << tmpitem.name << std::endl;
            //std::cout << tmpitem.create << " " << tmpitem.access << " " << tmpitem.modify << std::endl;
            //filevector->push_back(tmpitem);

        itemtype = 3;
        //std::cout << "sort name: " << fileitems->at(i).name << " isdirectory: " << fileitems->at(i).isdirectory << std::endl;
        if(fileitems->at(i).isdirectory)
            itemtype = 2;
        //std::cout << "sort itemtype: " << itemtype << std::endl;
        //std::cout << "currentitem.itemtext: " << currentitem.itemtext << std::endl;
        filteredlist->setItem(i, 0, new CheckTableItem(filteredlist, NULL, NULL, ""));
        filteredlist->setItemData(i, 0, &(currentitem.itemtext));
        //filteredlist->setItemData(i, 1, &itemtype);
        if(fileitems->at(i).gid == 0)
            filteredlist->setItemText(i, 1, FXString::value(globalid));
        else
            filteredlist->setItemText(i, 1, FXString::value(fileitems->at(i).gid));
        globalid = curid;
        filteredlist->setItemData(i, 2, curforimg);
        filteredlist->setItemText(i, 2, FXString(fileitems->at(i).name.c_str()));
        if(fileitems->at(i).isdeleted)
        {
            if(fileitems->at(i).isdirectory)
                filteredlist->setItemIcon(i, 2, deletedfoldericon);
            else
                filteredlist->setItemIcon(i, 2, deletedfileicon);
        }
        else
        {
            if(fileitems->at(i).isvirtual)
            {
                if(fileitems->at(i).isdirectory)
                    filteredlist->setItemIcon(i, 2, virtualfoldericon);
                else
                    filteredlist->setItemIcon(i, 2, virtualfileicon);
            }
            else
            {
                if(fileitems->at(i).isdirectory)
                    filteredlist->setItemIcon(i, 2, defaultfoldericon);
                else
                    filteredlist->setItemIcon(i, 2, defaultfileicon);
            }
        }
        filteredlist->setItemIconPosition(i, 2, FXTableItem::BEFORE);
        filteredlist->setItemText(i, 3, FXString(fileitems->at(i).path.c_str()));
        filteredlist->setItemData(i, 3, &(fileitems->at(i).layout));
        filteredlist->setItemText(i, 4, FXString(ReturnFormattingSize(fileitems->at(i).size).c_str()));
        filteredlist->setItemData(i, 4, &(currentitem.voloffset));
        filteredlist->setItemText(i, 5, FXString(fileitems->at(i).create.c_str()));
        filteredlist->setItemText(i, 6, FXString(fileitems->at(i).access.c_str()));
        filteredlist->setItemText(i, 7, FXString(fileitems->at(i).modify.c_str()));
     */ 
}

/*
long FilterView::AddRow(FXObject*, FXSelector, void*)
{
    filetypetable->insertRows(filetypetable->getNumRows(), 1);

    return 1;
}

FXString FilterView::ReturnFilterView()
{
    FXString carvetypestring = "";
    for(int i=0; i < filetypetable->getNumRows(); i++)
    {
        if(!filetypetable->getItemText(i, 0).empty())
        {
            carvetypestring += filetypetable->getItemText(i, 0) + "," + filetypetable->getItemText(i, 1) + "," + filetypetable->getItemText(i, 2) + "." + filetypetable->getItemText(i, 3) + "," + filetypetable->getItemText(i, 4) + "," + filetypetable->getItemText(i, 5) + "\n";
        }
    }

    return carvetypestring;
}
*/

void FilterView::LoadFilterView(FXString cursettings)
{
    /*
    FXArray<FXint> posarray;
    posarray.clear();
    int found = 0;
    posarray.append(-1);
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
    */
}
