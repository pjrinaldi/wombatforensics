#ifndef PROPERTIESWINDOW_H
#define PROPERTIESWINDOW_H

#include "wombatinclude.h"
#include "ui_propertieswindow.h"
#include "wombatdatabase.h"

namespace Ui {
class PropertiesWindow;
}

class PropertiesWindow : public QDialog
{
    Q_OBJECT

public:
    PropertiesWindow(WombatDatabase* wdata, QWidget* parent = 0);
    /*
    void UpdateAnalysisState(QString analysisState); // analysisStateEdit
    void UpdateFilesFound(QString filesFound); // filesFoundEdit
    void UpdateFilesProcessed(QString filesProcessed); // filesProcessedEdit
    void UpdateAnalysisTree(int parentIndex, QTreeWidgetItem *child); // analysisTreeView
    void UpdateMessageTable(QStringList msgList);
    void UpdateProgressBar(int progressValue); // progressBar
    void ClearTableWidget();
    */
    ~PropertiesWindow();


private slots:
    //void StepProgress(void);
    void HideClicked();
    //void JobClicked(QTreeWidgetItem* item);
signals:
    void HidePropertyWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);
private:
    Ui::PropertiesWindow *ui;
    //int counter;
    WombatDatabase* pdata;
};

Q_DECLARE_METATYPE(PropertiesWindow*)

#endif // PROPERTIESWINDOW_H
