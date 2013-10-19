#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QDir>
#include <QString>

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ExportDialog(QWidget *parent = 0, int curcheckcount = 0, int curlistcount = 0);
    ~ExportDialog();

private slots:
    void SelectDirectory(void);
    void ExportFiles(void);

private:
    Ui::ExportDialog *ui;
    int checkcount;
    int listcount;
    QWidget* parentwidget;
};

#endif // EXPORTDIALOG_H
