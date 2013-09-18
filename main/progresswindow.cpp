#include "progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent, WombatDatabase* wdata) : QDialog(parent), ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
    connect(ui->hideButton, SIGNAL(clicked()), this, SLOT(HideClicked()));
    connect(ui->analysisTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(JobClicked(QTreeWidgetItem*)));
    ui->analysisTreeWidget->hideColumn(1);
    ui->msgTableWidget->setCurrentCell(-1, -1, QItemSelectionModel::NoUpdate);
    counter = 0;
    pdata = wdata;
}

ProgressWindow::~ProgressWindow()
{
    //this->close();
    //delete ui;
}

void ProgressWindow::HideClicked()
{
    this->hide();
    emit HideProgressWindow(false);
}

void ProgressWindow::JobClicked(QTreeWidgetItem* item)
{
    int jobid = item->data(1,0).toInt();
    QStringList joblist = pdata->ReturnJobDetails(jobid);
    UpdateFilesFound(joblist[1]);
    UpdateFilesProcessed(joblist[2]);
    UpdateProgressBar(100);
    UpdateAnalysisState(joblist[3]);
    QStringList tmplist = pdata->ReturnMessageTableEntries(pdata->ReturnJobCaseID(jobid), pdata->ReturnJobEvidenceID(jobid), jobid);
    UpdateMessageTable(tmplist);
}

void ProgressWindow::UpdateAnalysisState(QString analysisState) // analysisStateEdit
{
    ui->analysisStateEdit->setText(analysisState);
}

void ProgressWindow::UpdateFilesFound(QString filesFound) // filesFoundEdit
{
    ui->filesFoundEdit->setText(filesFound);
}

void ProgressWindow::UpdateFilesProcessed(QString filesProcessed) // filesProcessedEdit
{
    ui->filesProcessedEdit->setText(filesProcessed);
}

void ProgressWindow::UpdateAnalysisTree(int parentIndex, QTreeWidgetItem *child) // analysisTreeWidget
{
    ui->analysisTreeWidget->collapseAll();
    ui->analysisTreeWidget->topLevelItem(parentIndex)->addChild(child);
    //ui->analysisTreeWidget->expand(itemAt(parentIndex)) // look online next time i'm online
    ui->analysisTreeWidget->setCurrentItem(child);
}
void ProgressWindow::ClearTableWidget()
{
    ui->msgTableWidget->clearContents();
}

void ProgressWindow::UpdateMessageTable(QStringList msglist)
{
    int currow = 0;
    ui->msgTableWidget->setCurrentCell(currow, 0, QItemSelectionModel::NoUpdate);
    for(int i=0; i < (msglist.size() / 2); i++)
    {
        ui->msgTableWidget->setItem(currow, 0, new QTableWidgetItem(msglist[(2*i)]));
        ui->msgTableWidget->setItem(currow, 1, new QTableWidgetItem(msglist[(2*i +1)]));
        currow++;
    }
}

void ProgressWindow::UpdateProgressBar(int progressValue) // progressBar
{
    ui->progressBar->setValue(progressValue);
}

void ProgressWindow::StepProgress(void)
{
    counter++;
}
