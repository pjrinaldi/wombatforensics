#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "ui_progresswindow.h"
#include <QDialog>

namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressWindow(QWidget *parent = 0);
    void UpdateAnalysisState(QString analysisState); // analysisStateEdit
    void UpdateFilesFound(QString filesFound); // filesFoundEdit
    void UpdateFilesProcessed(QString filesProcessed); // filesProcessedEdit
    void UpdateAnalysisTree(int parentIndex, QTreeWidgetItem *child); // analysisTreeView
    void UpdateMessageTable(QString msgType, QString msgValue); // msgTableWidget
    void UpdateProgressBar(int progressValue); // progressBar
    ~ProgressWindow();

public slots:
    void accept();

private slots:
    void StepProgress(void);
signals:
    void HideProgressWindow(bool checkstate);

private:
    Ui::ProgressWindow *ui;
    int counter;
};

Q_DECLARE_METATYPE(ProgressWindow*)

#endif // PROGRESSWINDOW_H
