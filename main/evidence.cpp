#include "evidence.h"

PluginMap EvidencePlugin::Initialize(WombatVariable wombatvariable)
{
    PluginMap mainmap;
    plugmap.clear();
    // QMenu setup
    plugmap.insert("addmenuitem", QStringList() << "Evidence" << "Add Evidence" << tr(":/basic/addevidence") << "Remove Evidence" << ":/basic/remevidence");
    plugmap.insert("addtoolbutton", QStringList() << "Add Evidence" << tr(":/basic/addevidence"));
    plugmap.insert("actions", QStringList() << "Add Evidence" << "Remove Evidence");
    mainmap.map = plugmap;
    mastermap.map = plugmap;
    return mainmap;
}

void EvidencePlugin::Run(QString input)
{
    fprintf(stderr, "Map Size: %d\n", mastermap.map.size());
    QStringList tmplist = mastermap.map["actions"].toStringList();

    if(input.compare("Add Evidence") == 0)
    {
        QString evidenceFilePath = QFileDialog::getOpenFileName((QWidget*)mastervariable.parent, tr("Select Evidence Item"), tr("./"));
    }
    /*
    foreach(QString tmpstring, tmplist)
    {
        if(input.compare(tmpstring) == 0)
            fprintf(stderr, "Run with input: %s\n", input.toStdString().c_str());
            */
        /*
        //QString evidenceFilePath = "/home/pasquale/Projects/TestImages/8-jpeg-search/8-jpeg-search.dd";
        QString evidenceFilePath = QFileDialog::getOpenFileName(this, tr("Select Evidence Item"), tr("./"));
        wombatprogresswindow->show();
        //QString evidenceFilePath = iEvidence->addEvidence();
        //QFileDialog *filedialog = new QFileDialog(0, "Select Evidence Item", "./");
        //filedialog->setFileMode(QFileDialog::ExistingFile);
        //filedialog->open(this, SLOT(dialogClosed(QString)));
        //
        fprintf(stderr, "Evidence FilePath: %s\n", evidenceFilePath.toStdString().c_str());
        if(evidenceFilePath != "")
        {
            wombatvariable.jobtype = 1; // add evidence
            // MIGHT BE AN ISSUE WHEN YOU OPEN MORE THAN 1 EVIDENCE ITEM... HAVE TO TEST IT OUT AND SEE WHAT HAPPENS
            QString evidenceName = evidenceFilePath.split("/").last();
            evidenceName += ".db";
            
            currentsleuthimages << setupSleuthKitImgDb(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath);
            setupSleuthKitBlackboard(sleuthkitplugin);
            wombatvariable.SetEvidenceID(wombatcasedata->InsertEvidence(evidenceName, evidenceFilePath, wombatvariable->GetCaseID()));
            wombatvariable.SetJobID(wombatcasedata->InsertJob(wombatvariable->GetJobType(), wombatvariable->GetCaseID(), wombatvariable->GetEvidenceID()));
            SleuthKitLogEntry(sleuthkitplugin, "Adding Evidence Started.");
            // UPDATE MESSAGETABLE(CASEID, EVIDENCEID, JOBID) WHEN JOB STARTS TO SHOW IT STARTED... WHICH SHOULD BE A SQL QUERY TO GET LOG ITEMS FOR IT.
            QString tmpString = evidenceName;
            tmpString += " - ";
            tmpString += QString::fromStdString(GetTime());
            QStringList tmpList;
            tmpList << tmpString << QString::number(wombatvariable.GetJobID());
            wombatprogresswindow->UpdateAnalysisTree(0, new QTreeWidgetItem(tmpList));
            wombatprogresswindow->UpdateFilesFound("0");
            wombatprogresswindow->UpdateFilesProcessed("0");
            wombatprogresswindow->UpdateAnalysisState("Adding Evidence to Database");
            //QList<QStringList> = wombatcasedata->ReturnLogMessages(wombatvariable.GetCaseID(), wombatvariable->GetEvidenceID(), wombatvariable->GetJobID());
            wombatprogresswindow->UpdateMessageTable("[INFO]", "Adding Evidence Started.");
            PluginRunner* prun = new PluginRunner(this, sleuthkitplugin, evidenceFilePath);
            prun->setAutoDelete(false);
            threadpool->start(prun);
            threadpool->waitForDone();
            //sleuthKitLoadEvidence(sleuthkitplugin, evidenceFilePath, wombatprogresswindow);
            // need to populate the directory tree entries
            //
            //QStandardItem* imageNode = GetCurrentImageDirectoryTree(sleuthkitplugin, currentcaseevidencepath, evidenceFilePath.split("/").last());
            //QStandardItem* currentroot = wombatdirmodel->invisibleRootItem();
            //currentroot->appendRow(imageNode);
            //currenttreeview->setModel(wombatdirmodel);
            
        }
    }*/
}
