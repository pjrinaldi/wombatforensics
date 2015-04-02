#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "wombatinclude.h"
#include "wombatvariable.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportDialog(QWidget *parent = 0, unsigned long long curcheckcount = 0, unsigned long long curlistcount = 0);
    ~ExportDialog();

signals:
    void FileExport(FileExportData* exportdata);

private slots:
    void SelectDirectory(void);
    void ExportFiles(void);

private:
    Ui::ExportDialog *ui;
    unsigned long long checkcount;
    unsigned long long listcount;
    QWidget* parentwidget;
};

#endif // EXPORTDIALOG_H
