#include "mboxviewer.h"

// Copyright 2013-2022 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

MBoxDialog::MBoxDialog(QWidget* parent) : QDialog(parent), ui(new Ui::MBoxDialog)
{
    ui->setupUi(this);
    ui->toolButton->setVisible(false);
    ui->label->setVisible(false);
    ui->mailtable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->mailtable->setHorizontalHeaderLabels({"ID", "From", "Date Time", "Subject", "Tag"});
    connect(ui->mailtable, SIGNAL(itemSelectionChanged()), this, SLOT(EmailSelected()), Qt::DirectConnection);

    QStringList taglist;
    taglist.clear();
    tagmenu = new QMenu(ui->mailtable);
    bookmarkfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(bookmarkfile.isOpen())
	taglist = QString(bookmarkfile.readLine()).split(",", Qt::SkipEmptyParts);
    bookmarkfile.close();
    for(int i=0; i < taglist.count(); i++)
    {
	QAction* tmpaction = new QAction(taglist.at(i), tagmenu);
	tmpaction->setIcon(QIcon(":/bar/addtotag"));
	tmpaction->setData(QVariant("t" + QString::number(i)));
	connect(tmpaction, SIGNAL(triggered()), this, SLOT(SetTag()));
	tagmenu->addAction(tmpaction);
    }
    tagmenu->addSeparator();
    QAction* remtagaction = new QAction("Remove Tag", tagmenu);
    remtagaction->setIcon(QIcon(":/bar/tag-rem"));
    connect(remtagaction, SIGNAL(triggered()), this, SLOT(RemoveTag()));
    tagmenu->addAction(remtagaction);
    ui->mailtable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mailtable, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(TagMenu(const QPoint &)), Qt::DirectConnection);
    QFile mboxtagfile;
    mboxtagfile.setFileName(wombatvariable.tmpmntpath + "mboxtags");
    if(!mboxtagfile.isOpen())
        mboxtagfile.open(QIODevice::ReadOnly | QIODevice::Text);
    mboxtaglist.clear();
    if(mboxtagfile.isOpen())
        mboxtaglist = QString(mboxtagfile.readLine()).split(",", Qt::SkipEmptyParts);
    mboxtagfile.close();
}

MBoxDialog::~MBoxDialog()
{
    delete ui;
}

void MBoxDialog::HideClicked()
{
    this->close();
}

void MBoxDialog::EmailSelected()
{
    QString mboxid = ui->mailtable->item(ui->mailtable->currentRow(), 0)->text().split("-m").at(0);
    QString layout = ui->mailtable->item(ui->mailtable->currentRow(), 0)->toolTip();
    QFile mboxfile(wombatvariable.tmpfilepath + mboxid + "-fhex");
    if(!mboxfile.isOpen())
        mboxfile.open(QIODevice::ReadOnly | QIODevice::Text);
    if(mboxfile.isOpen())
    {
        mboxfile.seek(layout.split(";").at(0).split(",").at(0).toULongLong());
        htmlvalue = mboxfile.read(layout.split(";").at(0).split(",").at(1).toULongLong());
        ui->mailcontent->setPlainText(htmlvalue);
        mboxfile.close();
    }
}

void MBoxDialog::closeEvent(QCloseEvent* e)
{
    e->accept();
}

void MBoxDialog::LoadMBoxFile(QString mboxid, QString mboxname)
{
    mboxparentname = mboxname;
    ui->mailtable->clear();
    ui->mailcontent->setPlainText("");
    QDir mailboxdir(wombatvariable.tmpmntpath + "mailboxes/");
    QStringList mailboxfiles = mailboxdir.entryList(QStringList() << QString(mboxid + "-m*.prop"), QDir::Files);
    ui->mailtable->setHorizontalHeaderLabels({"ID", "From", "Date Time", "Subject", "Tag"});
    ui->mailtable->setRowCount(mailboxfiles.count());
    for(int i=0; i < mailboxfiles.count(); i++)
    {
        QTableWidgetItem* tmpitem = new QTableWidgetItem(QString(mailboxfiles.at(i)).remove(".prop"));
        QFile propfile(wombatvariable.tmpmntpath + "mailboxes/" + mailboxfiles.at(i));
        if(!propfile.isOpen())
            propfile.open(QIODevice::ReadOnly | QIODevice::Text);
        if(propfile.isOpen())
        {
            while(!propfile.atEnd())
            {
                QString tmpstr = propfile.readLine();
                if(tmpstr.startsWith("From|"))
                    ui->mailtable->setItem(i, 1, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Date|"))
                    ui->mailtable->setItem(i, 2, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Subject|"))
                    ui->mailtable->setItem(i, 3, new QTableWidgetItem(tmpstr.split("|").at(1)));
                if(tmpstr.startsWith("Layout|"))
                    tmpitem->setToolTip(tmpstr.split("|").at(1));
            }
            propfile.close();
        }
        ui->mailtable->setItem(i, 0, tmpitem);
        QString tagstr = "";
        for(int j=0; j < mboxtaglist.count(); j++)
        {
            if(mboxtaglist.at(j).contains(QString(mailboxfiles.at(i)).remove(".prop")))
                tagstr = mboxtaglist.at(j).split("|", Qt::SkipEmptyParts).last();
        }
        ui->mailtable->setItem(i, 4, new QTableWidgetItem(tagstr));
    }
    this->show();
}

void MBoxDialog::TagMenu(const QPoint &pt)
{
    tagmenu->exec(ui->mailtable->mapToGlobal(pt));
}

void MBoxDialog::SetTag()
{
    // FOR MBOX, I NEED THE FILE ID, MAIL ID, AND MAIL MESSAGE
    QString curtag = "";
    QString mboxstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "|";
    if(ui->mailtable->item(ui->mailtable->currentRow(), 4) != NULL)
        curtag = mboxstring + ui->mailtable->item(ui->mailtable->currentRow(), 4)->text();
    mboxstring += tagaction->iconText(); // tag name
    ui->mailtable->setItem(ui->mailtable->currentRow(), 4, new QTableWidgetItem(tagaction->iconText()));
    if(!curtag.isEmpty())
	RemTag("mbox", curtag);
    AddTag("mbox", mboxstring); // this adds the id and tag to the mboxtags file // add htmlentry and htmlvalue to this function...
    //RemoveArtifactFile("mbox", idkeyvalue);
    htmlentry = "";
    htmlentry += "<td style='" + ReturnCssString(11) + "' id='" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "'>";
    htmlentry += "<table style='" + ReturnCssString(2) + "' width=300px'><tr style='" + ReturnCssString(3) + "'><th style='" + ReturnCssString(6) + "' colspan='2'>Email Message " + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text().split("-m").at(1) + "</th></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>From:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 1)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Date:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 2)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Subject:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 3)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Mbox Id:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(12) + "'><td style='" + ReturnCssString(13) + "'>Mbox File Name:</td><td style='" + ReturnCssString(14) + "'><span style='word-wrap:break-word;'>" + mboxparentname + "</span></td></tr>";
    htmlentry += "<tr style='" + ReturnCssString(5) + "'><td style='" + ReturnCssString(13) + "'>&nbsp;</td><td style='" + ReturnCssString(7) + "'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='./mbox/" + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "'>Link</a></td></tr>";
    htmlentry += "</table></td>";

    QString htmlcontent = "";
    htmlcontent = "<html><body style='" + ReturnCssString(0) + "'>";
    htmlcontent += "<div style='" + ReturnCssString(1) + "'>MBox Email Analysis - " + ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "</div><br/>";
    htmlcontent += "<pre>";
    htmlcontent += htmlvalue;
    htmlcontent += "</pre>";
    htmlcontent += "</body></html>";

    AddFileItem(tagaction->iconText(), htmlentry);
    CreateArtifactFile("mbox", ui->mailtable->item(ui->mailtable->currentRow(), 0)->text(), htmlcontent);
    // ADD TO PREVIEW REPORT
    //RemoveFileItem(curindex.sibling(curindex.row(), 11).data().toString());
    //AddFileItem(tagname, filestr);
    //CreateArtifactFile("registry", curtag, htmlvalue);
}

void MBoxDialog::RemoveTag()
{
    QString mboxstring = "";
    QAction* tagaction = qobject_cast<QAction*>(sender());
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 0)->text() + "|";
    mboxstring += ui->mailtable->item(ui->mailtable->currentRow(), 4)->text();
    ui->mailtable->item(ui->mailtable->currentRow(), 4)->setText("");
    RemTag("mbox", mboxstring);
    RemoveFileItem(ui->mailtable->item(ui->mailtable->currentRow(), 0)->text());
    RemoveArtifactFile("mbox", ui->mailtable->item(ui->mailtable->currentRow(), 0)->text());
    /*
    // REMOVE FROM PREVIEW REPORT
    RemoveFileItem(idkeyvalue);
    RemoveArtifactFile("mbox", idkeyvalue);
    */
}
