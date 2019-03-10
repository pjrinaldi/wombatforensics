#include "remevidencedialog.h"

// Copyright 2015 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

RemEvidenceDialog::RemEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::RemEvidenceDialog)
{
    ui->setupUi(this);
    //ui->removebutton->setEnabled(false);
    //ui->startbutton->setEnabled(false);
    //connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(SelectEvidence()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));
    //connect(ui->startbutton, SIGNAL(clicked()), this, SLOT(StartProcess()));
    //connect(ui->evidencelist, SIGNAL(itemSelectionChanged()), this, SLOT(UpdateButtons()));

    // POPULATE EVIDENCE LIST
    for(int i=0; i < evidencelist.count(); i++)
        ui->evidencelist->addItem(evidencelist.at(i));
    /*
    parentwidget = parent;
    checkcount = curcheckcount;
    QString checktext = QString("Checked (") + QString::number(checkcount) + QString(")");
    listcount = curlistcount;
    QString listtext = QString("All (") + QString::number(listcount) + QString(")");
    ui->checkedFileRadioButton->setText(checktext);
    ui->listedFileRadioButton->setText(listtext);
    ui->exportButton->setEnabled(false);
    ui->selectedFileRadioButton->setChecked(true);
    if(checkcount <= 0)
        ui->checkedFileRadioButton->setEnabled(false);
    else
        ui->checkedFileRadioButton->setEnabled(true);
    if(listcount <= 0)
        ui->listedFileRadioButton->setEnabled(false);
    else
        ui->listedFileRadioButton->setEnabled(true);
    ui->includePathRadioButton->setChecked(true);
    connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(SelectDirectory()));
    connect(ui->exportButton, SIGNAL(clicked()), this, SLOT(ExportFiles()));
   */
    /*
    QString abouttext = "<h3>About WombatForensics v0.1</h3>";
    abouttext += "<h5>License: GPLv2</h5>";
    abouttext += "<4>Copyright 2015 Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<h5>Email: pjrinaldi@gmail.com</h5>";
    abouttext += "<p>This program incorporates modified code from the qhexedit2 project to implement the hexviewers.";
    abouttext += "qhexedit2 is provided under the gplv2.1. I also used Qt5.11, the sleuthkit c/c++ functions, and the treepata icons.";
    abouttext += "The wombat icon was designed by Lindsey Pargman and a lot of the design implementation ideas were brainstormed with Mark Smith.</p>";
    ui->label->setText(abouttext);
    this->hide();
    */
}

RemEvidenceDialog::~RemEvidenceDialog()
{
}

/*
void AddEvidenceDialog::SelectEvidence()
{
    QFileDialog addeviddialog(this, tr("Add Evidence Item"), QDir::homePath());
    addeviddialog.setFileMode(QFileDialog::ExistingFile);
    addeviddialog.setLabelText(QFileDialog::Accept, "Add");
    QString evidfilename = "";
    if(addeviddialog.exec())
        evidfilename = addeviddialog.selectedFiles().first();
    //QString evidfilename = QFileDialog::getOpenFileName(this, tr("Add Evidence Item"), QDir::homePath());
    // check for evidence image
    if(evidfilename.toLower().contains(".dd") || evidfilename.toLower().contains(".e01") || evidfilename.toLower().contains(".000") || evidfilename.toLower().contains(".001") || evidfilename.toLower().contains(".aff"))
    {
        // it's an evidence image so process...
        ui->evidencelist->addItem(evidfilename);
        ui->startbutton->setEnabled(true);
    }
    else if(evidfilename.isNull())
    {
        qDebug() << "cancelled by the user.";
    }
    else
    {
        QMessageBox::warning(this, tr("Unsupported Format"), tr("Unfortunately, this format is not supported YET, it should be supported by v0.2"), QMessageBox::Ok);
        qDebug() << "Unfortunately this format is not supported YET, it should be supported by v0.2.";
    }
}

void AddEvidenceDialog::UpdateButtons()
{
    if(ui->evidencelist->selectedItems().count() > 0)
        ui->removebutton->setEnabled(true);
}
*/

void RemEvidenceDialog::RemoveEvidence()
{
    qDebug() << ui->evidencelist->selectedItems();
    //qDeleteAll(ui->evidencelist->selectedItems());
    /*
    if(ui->evidencelist->count() == 0)
        ui->startbutton->setEnabled(false);
    */
}

void RemEvidenceDialog::Cancel()
{
    this->close();
}
/*
void AddEvidenceDialog::StartProcess()
{
    evidencelist.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
    {
        evidencelist.append(ui->evidencelist->item(i)->text());
        this->close();
    }
}

void AddEvidenceDialog::dragEnterEvent(QDragEnterEvent* e)
{
    if(e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void AddEvidenceDialog::dropEvent(QDropEvent* e)
{
    foreach (const QUrl &url, e->mimeData()->urls())
    {
        ui->evidencelist->addItem(url.toLocalFile());
        ui->startbutton->setEnabled(true);
        //qDebug() << "dropped filepath:" << url.toLocalFile();
    }
}
*/
