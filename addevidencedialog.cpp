#include "addevidencedialog.h"

// Copyright 2013-2020 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

AddEvidenceDialog::AddEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::AddEvidenceDialog)
{
    ui->setupUi(this);
    ui->removebutton->setEnabled(false);
    ui->startbutton->setEnabled(false);
    connect(ui->addbutton, SIGNAL(clicked()), this, SLOT(SelectEvidence()));
    connect(ui->removebutton, SIGNAL(clicked()), this, SLOT(RemoveEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));
    connect(ui->startbutton, SIGNAL(clicked()), this, SLOT(StartProcess()));
    connect(ui->evidencelist, SIGNAL(itemSelectionChanged()), this, SLOT(UpdateButtons()));
}

AddEvidenceDialog::~AddEvidenceDialog()
{
}

void AddEvidenceDialog::SelectEvidence()
{
    QFileDialog addeviddialog(this, tr("Add Evidence Item"), QDir::homePath());
    addeviddialog.setFileMode(QFileDialog::ExistingFile);
    addeviddialog.setLabelText(QFileDialog::Accept, "Add");
    addeviddialog.setOption(QFileDialog::DontUseNativeDialog, true);
    QString evidfilename = "";
    bool alreadyadded = false;
    if(addeviddialog.exec())
        evidfilename = addeviddialog.selectedFiles().first();
    for(int i=0; i < existingevidence.count();i++)
    {
        if(existingevidence.at(i).contains(evidfilename))
            alreadyadded = true;
    }
    if(alreadyadded == false)
    {
        if(evidfilename.toLower().contains(".dd") || evidfilename.toLower().contains(".e01") || evidfilename.toLower().contains(".000") || evidfilename.toLower().contains(".001") || evidfilename.toLower().contains(".aff") || evidfilename.toLower().contains(".image") || evidfilename.toLower().contains(".dmg"))
        {
            // it's an evidence image so process...
            ui->evidencelist->addItem(evidfilename);
            ui->startbutton->setEnabled(true);
            // POPUP HERE FOR EVID ITEM WOULD BE GOOD....
            // LET's TEST IT OUT...
            // IF I DON'T WRITE THE STAT/PROP FILE'S HERE, WHERE I WILL STORE PASSWORDS FOR MULTIPLE EVIDENCE ITEMS WITH MULTIPLE ENCRYPTED POOL'S/FS'S
            // IF I DO WRITE THE STAT/PROP FILE'S HERE, HOW I WILL NO TO DELETE THE FILES IF THE USER CANCEL'S THE OPERATION...
            qDebug() << "encryption test begins...";
            const TSK_TCHAR** images;
            std::vector<std::string> fullpathvector;
            fullpathvector.clear();
            fullpathvector.push_back(evidfilename.toStdString());
            images = (const char**)malloc(fullpathvector.size()*sizeof(char*));
            for(uint i=0; i < fullpathvector.size(); i++)
                images[i] = fullpathvector[i].c_str();
            TSK_IMG_INFO* imginfo = NULL;
            imginfo = tsk_img_open(1, images, TSK_IMG_TYPE_DETECT, 0);
            if(imginfo == NULL)
                qDebug() << "imginfo is null...";
            free(images);
            TSK_VS_INFO* vsinfo = NULL;
            const TSK_POOL_INFO* poolinfo = nullptr;
            TSK_FS_INFO* fsinfo = NULL;
            if(imginfo != NULL)
                vsinfo = tsk_vs_open(imginfo, 0, TSK_VS_TYPE_DETECT);
            if(vsinfo == NULL) // no volume, single fs is all there is...
            {
                qDebug() << "vsinfo is null...";
                qDebug() << "tsk can't open a pool from an image only...";
                // NEED TO FIGURE OUT IF THERE IS AN INDICATOR OF POOL AND APFS SIGNATURE THAT I COULD DETECT IN THE IMAGE...
                poolinfo = tsk_pool_open_img_sing(imginfo, 0, TSK_POOL_TYPE_APFS);
                if(poolinfo == nullptr) // doesn't contain a pool...
                {
                    qDebug() << "pool is null...";
                    fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
                    if(fsinfo == NULL) // unrecognized fs..
                    {
                    }
                    else
                    {
                        if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
                            qDebug() << "encrypted FS: prompt for password...";
                        else
                            qDebug() << "not encrypted FS...";
                    }
                }
                else // has a pool
                {
                    if(poolinfo->num_vols > 0)
                    {
                        for(int i=0; i < poolinfo->num_vols; i++)
                        {
                            TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[i];
                            if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                                qDebug() << "encrypted POOL VOLUME: prompt for password...";
                            else
                                qDebug() << "not encrypted pool...";
                        }
                    }
                }
            }
            else // contains volume... repeat above for each partition...
            {
                qDebug() << "vsinfo is not null....";
                const TSK_VS_PART_INFO* partinfo = NULL;
                if(vsinfo->part_count > 0)
                {
                    for(uint32_t i=0; i < vsinfo->part_count; i++)
                    {
                        partinfo = tsk_vs_part_get(vsinfo, i);
                        if(partinfo != NULL)
                        {
                            if(partinfo->flags == 0x02 || partinfo->flags == 0x04) // unallocated partition or meta entry
                            {
                            }
                            else if(partinfo->flags == 0x01) // allocated partition
                            {
                                fsinfo = NULL;
                                poolinfo = nullptr;
                                poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                                if(poolinfo == nullptr)
                                {
                                    fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
                                }
                                else
                                {
                                    if(poolinfo->num_vols > 0)
                                    {
                                        TSK_IMG_INFO* curimginfo = NULL;
                                        for(int j=0; j < poolinfo->num_vols; j++)
                                        {
                                            TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[j];
                                            curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                                            if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
                                            {
                                                qDebug() << "encrypted POOL VOLUME; prompt for password...";
                                                fsinfo = tsk_fs_open_img_decrypt(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT, "encrypted");
                                            }
                                            else
                                            {
                                                qDebug() << "not encrypted pool volume...";
                                                fsinfo = tsk_fs_open_img(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT);
                                            }
                                            if(fsinfo != NULL)
                                            {
                                                if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
                                                    qDebug() << "encrypted FS: prompt for password...";
                                                else
                                                    qDebug() << "not encrypted FS...";
                                            }
                                            else
                                            {
                                                qDebug() << "fsinfo is null for some reason...";
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                            }
                        }
                    }
                }
            }
        }
        else if(evidfilename.isNull())
        {
            qDebug() << "cancelled by the user.";
        }
        else
        {
            QMessageBox::warning(this, tr("Unsupported Format"), tr("Unfortunately, this format is not supported YET, it should be supported by v0.5"), QMessageBox::Ok);
            qDebug() << "Unfortunately this format is not supported YET, it should be supported by v0.5.";
        }
    }
    else
        QMessageBox::warning(this, tr("Already Added"), tr("The evidence has already been added to the case, please select another item."), QMessageBox::Ok);
}

void AddEvidenceDialog::UpdateButtons()
{
    if(ui->evidencelist->selectedItems().count() > 0)
        ui->removebutton->setEnabled(true);
}

void AddEvidenceDialog::RemoveEvidence()
{
    qDeleteAll(ui->evidencelist->selectedItems());
    if(ui->evidencelist->count() == 0)
        ui->startbutton->setEnabled(false);
}

void AddEvidenceDialog::Cancel()
{
    newevidence.clear();
    this->close();
}

void AddEvidenceDialog::StartProcess()
{
    newevidence.clear();
    for(int i=0; i < ui->evidencelist->count(); i++)
        newevidence.append(ui->evidencelist->item(i)->text());
    this->close();
}

void AddEvidenceDialog::dragEnterEvent(QDragEnterEvent* e)
{
    if(e->mimeData()->hasUrls())
        e->acceptProposedAction();
}

void AddEvidenceDialog::dropEvent(QDropEvent* e)
{
    bool alreadyadded = false;
    foreach(const QUrl &url, e->mimeData()->urls())
    {
        for(int i=0; i < existingevidence.count(); i++)
        {
            if(existingevidence.at(i).contains(url.toLocalFile().split("/").last()))
                alreadyadded = true;
        }
        if(alreadyadded == false)
        {
            ui->evidencelist->addItem(url.toLocalFile());
            ui->startbutton->setEnabled(true);
        }
    }
}
