#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "wombatinclude.h"
#include "ui_progresswindow.h"
#include "wombatdatabase.h"

namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog
{
    Q_OBJECT

public:
    ProgressWindow(WombatDatabase* wdata, QWidget* parent = 0);
    void UpdateAnalysisState(QString analysisState); // analysisStateEdit
    void UpdateFilesFound(QString filesFound); // filesFoundEdit
    void UpdateFilesProcessed(QString filesProcessed); // filesProcessedEdit
    void UpdateAnalysisTree(int parentIndex, QTreeWidgetItem *child); // analysisTreeView
    void UpdateMessageTable(QStringList msgList);
    void UpdateProgressBar(int progressValue); // progressBar
    void ClearTableWidget();
    ~ProgressWindow();


private slots:
    void StepProgress(void);
    void HideClicked();
    void JobClicked(QTreeWidgetItem* item);
signals:
    void HideProgressWindow(bool checkstate);

private:
    Ui::ProgressWindow *ui;
    int counter;
    WombatDatabase* pdata;
};

Q_DECLARE_METATYPE(ProgressWindow*)

#endif // PROGRESSWINDOW_H
