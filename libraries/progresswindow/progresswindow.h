#ifndef PROGRESSWINDOW_H
#define PROGRESSWINDOW_H

#include "main/interfaces.h"
#include <QDialog>
#include <QtPlugin>
#include <QObject>

namespace Ui {
class ProgressWindow;
}

class ProgressWindow : public QDialog, public ProgressWindowInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "wombat.ProgressWindowInterface" FILE "progresswindow.json")
    Q_INTERFACES(ProgressWindowInterface)
    
public:
    explicit ProgressWindow(QWidget *parent = 0);
    ~ProgressWindow();
    void UpdateProgressBar(int progressValue); // progressBar
    void UpdateAnalysisState(QString stateValue); // analysisStateEdit
    void UpdateFilesFound(QString filesfoundValue); // filesFoundEdit
    void UpdateFilesProcessed(QString filesprocessedValue); // filesProcessedEdit
    //void UpdateJobTreeView // analysisTreeView
    void UpdateMessages(QString msgType, QString msgValue); // msgTableWidget
    
private:
    Ui::ProgressWindow *ui;
};

#endif // PROGRESSWINDOW_H
