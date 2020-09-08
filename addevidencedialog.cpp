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
        if(evidfilename.toLower().endsWith(".dd") || evidfilename.toLower().endsWith(".e01") || evidfilename.toLower().endsWith(".000") || evidfilename.toLower().endsWith(".001") || evidfilename.toLower().endsWith(".aff") || evidfilename.toLower().endsWith(".image") || evidfilename.toLower().endsWith(".dmg") || evidfilename.toLower().endsWith(".sfs"))
        {
            // it's an evidence image so process...
            ui->evidencelist->addItem(evidfilename);
            if(evidfilename.toLower().endsWith(".sfs"))
            {
                // need to mount and provide access to the raw dd file...
                QString mntstr = "squashfuse " + evidfilename + " " + wombatvariable.imgdatapath;
                //qDebug() << "mntstr:" << mntstr;
		xmntprocess = new QProcess();
		connect(xmntprocess, SIGNAL(readyReadStandardOutput()), this, SLOT(ReadXMountOut()), Qt::QueuedConnection);
		connect(xmntprocess, SIGNAL(readyReadStandardError()), this, SLOT(ReadXMountErr()), Qt::QueuedConnection);
		//xmntprocess->setProgram(mntstr);
		xmntprocess->start(mntstr);
                //evidfilename = wombatvariable.imgdatapath + evidfilename.split("/").last();
                //qDebug() << "evidfilename:" << evidfilename;
                evidfilename = wombatvariable.imgdatapath + evidfilename.split("/").last().split(".sfs").first() + ".dd";
                xmntprocess->waitForFinished(-1);
            }
            ui->startbutton->setEnabled(true);
            //qDebug() << "evidfilename:" << evidfilename;
            // POPUP HERE FOR EVID ITEM WOULD BE GOOD....
            // LET's TEST IT OUT...
            // IF I DON'T WRITE THE STAT/PROP FILE'S HERE, WHERE I WILL STORE PASSWORDS FOR MULTIPLE EVIDENCE ITEMS WITH MULTIPLE ENCRYPTED POOL'S/FS'S
            // IF I DO WRITE THE STAT/PROP FILE'S HERE, HOW I WILL NO TO DELETE THE FILES IF THE USER CANCEL'S THE OPERATION...
            //qDebug() << "encryption test begins...";
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
                qDebug() << "imginfo is null..." << tsk_error_get_errstr();
            //else
            //    qDebug() << "The waitforfinished works...";
            free(images);
            TSK_VS_INFO* vsinfo = NULL;
            const TSK_POOL_INFO* poolinfo = nullptr;
            TSK_FS_INFO* fsinfo = NULL;
            if(imginfo != NULL)
                vsinfo = tsk_vs_open(imginfo, 0, TSK_VS_TYPE_DETECT);
            if(vsinfo == NULL) // no volume, single fs is all there is...
            {
                //qDebug() << "vsinfo is null...";
                //qDebug() << "tsk can't open a pool from an image only...";
                // NEED TO FIGURE OUT IF THERE IS AN INDICATOR OF POOL AND APFS SIGNATURE THAT I COULD DETECT IN THE IMAGE...
                poolinfo = tsk_pool_open_img_sing(imginfo, 0, TSK_POOL_TYPE_APFS);
                if(poolinfo == nullptr) // doesn't contain a pool...
                {
                    // THIS METHODOLOGY WON'T WORK CAUSE YOU CAN'T GET TSK_FS_INFO IF IT IS ENCRYPTED TO TELL IF IT IS ENCRYPTED OR NOT...
                    /*
                    //qDebug() << "pool is null...";
                    fsinfo = tsk_fs_open_img(imginfo, 0, TSK_FS_TYPE_DETECT);
                    if(fsinfo == NULL) // unrecognized fs..
                    {
                    }
                    else
                    {
                        if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
			{
			    qDebug() << evidfilename << "v0" << "p0";
                            //qDebug() << "encrypted FS: prompt for password...";
			}
                        //else
                        //    qDebug() << "not encrypted FS...";
                    }*/
                }
                else // has a pool
                {
                    if(poolinfo->num_vols > 0)
                    {
			TSK_IMG_INFO* curimginfo = NULL;
                        for(int i=0; i < poolinfo->num_vols; i++)
                        {
                            TSK_POOL_VOLUME_INFO curpoolvol = poolinfo->vol_list[i];
			    curimginfo = poolinfo->get_img_info(poolinfo, curpoolvol.block);
                            if(curpoolvol.flags & TSK_POOL_VOLUME_FLAG_ENCRYPTED)
			    {
                                // FOR NOW JUST STORE PASSWORD IN PASSWORD HASH...
				//qDebug() << "check password, if fs == null, then it didn't work...";
                                // NEED TO STORE IN HASH, ALSO POSSIBLY ALLOW IT TO BE CHANGED SOMEHOW...
                                //qDebug() << "encrypted POOL VOLUME: prompt for password...";
				//qDebug() << evidfilename << "v0" << "p" << i;
                                QString curitem = evidfilename.split("/", Qt::SkipEmptyParts).last() + "-v0" + "-p" + QString::number(i);
                                bool ok;
                                QString text = QInputDialog::getText(this, QString("Encrypted File System"), QString(curitem + "\nEnter Password:"), QLineEdit::Normal, "", &ok);
                                if (ok)
                                    passwordhash.insert(curitem, text);
                                fsinfo = tsk_fs_open_img_decrypt(curimginfo, 0, TSK_FS_TYPE_APFS_DETECT, text.toStdString().c_str());
			    }
                            else
			    {
                                // THIS METHODOLOGY WON'T WORK CAUSE YOU CAN'T GET TSK_FS_INFO IF IT IS ENCRYPTED TO TELL IF IT IS ENCRYPTED OR NOT...
                                /*
                                //qDebug() << "not encrypted pool...";
                                fsinfo = tsk_fs_open_img(curimginfo, 0, TSK_FS_TYPE_APFS_DETECT);
                                if(fsinfo != NULL)
                                {
                               	    if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
				    {
					qDebug() << evidfilename << "v0" << "p" << i;
                                        qDebug() << "encrypted FS: prompt for password...";
				    }
                                }*/
			    }
                        }
			tsk_img_close(curimginfo);
                    }
                }
            }
            else // contains volume... repeat above for each partition...
            {
                //qDebug() << "vsinfo is not null....";
                const TSK_VS_PART_INFO* partinfo = NULL;
                if(vsinfo->part_count > 0)
                {
                    for(uint32_t i=0; i < vsinfo->part_count; i++)
                    {
                        partinfo = tsk_vs_part_get(vsinfo, i);
                        if(partinfo != NULL)
                        {
                            if(partinfo->flags == 0x01) // allocated partition
                            {
                                fsinfo = NULL;
                                poolinfo = nullptr;
                                poolinfo = tsk_pool_open_sing(partinfo, TSK_POOL_TYPE_DETECT);
                                // THIS METHODOLOGY WON'T WORK CAUSE YOU CAN'T GET TSK_FS_INFO IF IT IS ENCRYPTED TO TELL IF IT IS ENCRYPTED OR NOT...
                                if(poolinfo == nullptr)
                                {
                                    /*
                                    fsinfo = tsk_fs_open_vol(partinfo, TSK_FS_TYPE_DETECT);
				    if(fsinfo != NULL)
				    {
					if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
					{
					    qDebug() << "encrypted FS: prompt for password...";
					    qDebug() << evidfilename << "v0" << "f" << i;
					}
				    }
                                    */
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
                                                //qDebug() << "encrypted POOL VOLUME: prompt for password...";
		                		//qDebug() << evidfilename << "v0" << "p" << i;
                                                //qDebug() << "password:" << text;
						// I + J IS EQUIVALENT OF INCREMENTING PARTINT++ BY 1 
					    	//qDebug() << evidfilename << "v0" << "f" << i + j;
						//qDebug() << "prompt here and store password...";
						//qDebug() << "check password, if fs == null, then it didn't work...";
						//qDebug() << "if it worked, store in passwordhash.";
                                                QString curitem = evidfilename.split("/", Qt::SkipEmptyParts).last() + "-v0" + "-p" + QString::number(i + j);
                                                bool ok;
                                                QString text = QInputDialog::getText(this, QString("Encrypted File System"), QString(curitem + "\nEnter Password:"), QLineEdit::Normal, "", &ok);
                                                if (ok)
                                                    passwordhash.insert(curitem, text);
                                                fsinfo = tsk_fs_open_img_decrypt(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT, text.toStdString().c_str());
                                            }
                                            //else
                                            //{
                                                //qDebug() << "not encrypted pool volume...";
                                                //fsinfo = tsk_fs_open_img(curimginfo, partinfo->start * curimginfo->sector_size, TSK_FS_TYPE_APFS_DETECT);
                                            //}
                                            // THIS METHODOLOGY WON'T WORK CAUSE YOU CAN'T GET TSK_FS_INFO IF IT IS ENCRYPTED TO TELL IF IT IS ENCRYPTED OR NOT...
                                            /*
                                            if(fsinfo != NULL)
                                            {
                                                if(fsinfo->flags & TSK_FS_INFO_FLAG_ENCRYPTED)
						{
					    	    qDebug() << evidfilename << "v0" << "f" << i;
                                                    qDebug() << "encrypted FS: prompt for password...";
						}
                                            }*/
                                        }
					tsk_img_close(curimginfo);
                                    }
                                }
                            }
                        }
                    }
                }
            }
	    tsk_fs_close(fsinfo);
	    tsk_pool_close(poolinfo);
	    tsk_vs_close(vsinfo);
	    tsk_img_close(imginfo);
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
    // update passwordhash here
    qDeleteAll(ui->evidencelist->selectedItems());
    if(ui->evidencelist->count() == 0)
        ui->startbutton->setEnabled(false);
}

void AddEvidenceDialog::Cancel()
{
    newevidence.clear();
    // clear passwordhash
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
void AddEvidenceDialog::ReadXMountOut()
{
    qWarning() << xmntprocess->readAllStandardOutput();
}

void AddEvidenceDialog::ReadXMountErr()
{
    qWarning() << xmntprocess->readAllStandardError();
}
