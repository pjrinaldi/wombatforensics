#include "verevidencedialog.h"

// Copyright 2013-2019 Pasquale J. Rinaldi, Jr.
// Distrubted under the terms of the GNU General Public License version 2

VerEvidenceDialog::VerEvidenceDialog(QWidget* parent) : QDialog(parent), ui(new Ui::VerEvidenceDialog)
{
    //ImageSignals* imgsignals = new ImageSignals();
    ui->setupUi(this);
    ui->progressBar->setVisible(false);
    connect(ui->verifybutton, SIGNAL(clicked()), this, SLOT(VerifyEvidence()));
    connect(ui->cancelbutton, SIGNAL(clicked()), this, SLOT(Cancel()));
    connect(imgsignals, SIGNAL(VerUpdate(QString, qint64)), this, SLOT(UpdateProgress(QString, qint64)), Qt::QueuedConnection);

    // POPULATE EVIDENCE LIST
    //qDebug() << "existing evidence:" << existingevidence;
    for(int i=0; i < existingforimglist.count(); i++)
	ui->evidencelist->addItem(existingforimglist.at(i)->ImgPath());
    /*
    for(int i=0; i < existingevidence.count(); i++)
        ui->evidencelist->addItem(existingevidence.at(i));
    */
}

VerEvidenceDialog::~VerEvidenceDialog()
{
}

void VerEvidenceDialog::UpdateProgress(QString pname, qint64 bytesread)
{
    QLabel* curlabel = this->findChild<QLabel*>(pname);
    //QProgressBar* curbar = this->findChild<QProgressBar*>(pname);
    //qDebug() << "curbar objectname:" << curbar->objectName();
    //qDebug() << "bytes read:" << bytesread;
    qint64 imgsize = 0;
    for(int i=0; i < existingforimglist.count(); i++)
    {
        if(existingforimglist.at(i)->ImgPath().contains(pname))
        {
            imgsize = existingforimglist.at(i)->Size();
            //qDebug() << "img size:" << existingforimglist.at(i)->Size();
            break;
        }
        //else
        //    qDebug() << "error in my logic to get forimg size.";
    }
    int curpercent = ((float)bytesread / (float)imgsize) * 100.0;
    curlabel->setText(pname + ": " + QString::number(bytesread) + " of " + QString::number(imgsize) + " " + QString::number(curpercent) + "%");
    //curbar->setValue(curpercent);
}

void VerEvidenceDialog::VerifyEvidence()
{
    QStringList tmplist;
    tmplist.clear();
    for(int i=0; i < ui->evidencelist->selectedItems().count(); i++)
    {
        /**/
        // ADD PROGRESS BARS HERE
        QLabel* tmplabel = new QLabel(this);
        tmplabel->setObjectName(ui->evidencelist->selectedItems().at(i)->text().split("/").last());
        QString labeltext = "Start Verification for " + ui->evidencelist->selectedItems().at(i)->text().split("/").last();
        tmplabel->setText(labeltext);
        QPushButton* tmpbutton = new QPushButton("Cancel Verification", this);
        tmpbutton->setObjectName(ui->evidencelist->selectedItems().at(i)->text().split("/").last() + "but");
        connect(tmpbutton, SIGNAL(clicked()), this, SLOT(CancelVer()));
        QHBoxLayout* hlayout = new QHBoxLayout(this);
        hlayout->addWidget(tmplabel);
        hlayout->addStretch();
        hlayout->addWidget(tmpbutton);
        ((QVBoxLayout*)ui->progressgroupbox->layout())->addLayout(hlayout);
        //ui->progressgroupbox->layout()->addWidget(tmplabel);
        //QProgressBar* tmpbar = new QProgressBar(this);
	//tmpbar->
        //qDebug() << "progressbar objectname:" << ui->evidencelist->selectedItems().at(i)->text().split("/").last();
        //tmpbar->setObjectName(ui->evidencelist->selectedItems().at(i)->text().split("/").last());
        //ui->progressgroupbox->layout()->addWidget(tmpbar);
        //tmpbar->
        // will have to be like the tmp menu's, which i can access by some value maybe
        //ui->progressgroupbox
        /**/
        tmplist.append(ui->evidencelist->selectedItems().at(i)->text());
    }

    connect(&verifywatcher, SIGNAL(finished()), this, SLOT(FinishVerify()), Qt::QueuedConnection);
    //connect(this, SIGNAL(CancelVerWatcher()), &verifywatcher, SLOT(cancel()));
    verfuture = QtConcurrent::mapped(tmplist, Verify);
    verifywatcher.setFuture(verfuture);
    //emit VerEvid(tmplist); // (Goes to WombatForensics)
    //this->hide();
    //emit HideVerifyWindow(false);
}

void VerEvidenceDialog::CancelVer()
{
    QPushButton* tagaction = qobject_cast<QPushButton*>(sender());
    qDebug() << "Cancelled pressed:" << tagaction->objectName();
    verifywatcher.cancel();
    //emit CancelVerWatcher(tagaction->objectName);
    //emit CancelVerWatcher();
}
void VerEvidenceDialog::Cancel()
{
    this->hide();
    emit HideVerifyWindow(false);
    //this->close();
}

void VerEvidenceDialog::Show()
{
    ui->evidencelist->clear();
    for(int i=0; i < existingforimglist.count(); i++)
	ui->evidencelist->addItem(existingforimglist.at(i)->ImgPath());
    this->show();
}
void VerEvidenceDialog::FinishVerify()
{
    QString resultstring = "";
    for(int i=0; i < verfuture.resultCount(); i++)
    {
        resultstring += QString::fromStdString(verfuture.resultAt(i)) + "\n";
    }
    QMessageBox::information(this, "Finished", " " + resultstring, QMessageBox::Ok);
}
