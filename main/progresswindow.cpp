#include "progresswindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ProgressWindow)
{
    ui->setupUi(this);
    ui->analysisTreeWidget->hideColumn(1);
    ui->msgTableWidget->setRowCount(5);
    ui->msgTableWidget->setCurrentCell(0, 0, QItemSelectionModel::NoUpdate);
}

ProgressWindow::~ProgressWindow()
{
    delete ui;
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

void ProgressWindow::UpdateMessageTable(QString msgType, QString msgValue) // msgTableWidget
{
    ui->msgTableWidget->setItem(ui->msgTableWidget->currentRow() + 1, 0, new QTableWidgetItem(msgType));
    ui->msgTableWidget->setItem(ui->msgTableWidget->currentRow() + 1, 1, new QTableWidgetItem(msgValue));
}

void ProgressWindow::UpdateProgressBar(int progressValue) // progressBar
{
    ui->progressBar->setValue(progressValue);
}
