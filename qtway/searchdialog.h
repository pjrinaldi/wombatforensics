#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <QtCore>
#include "qhexedit.h"

namespace Ui {
    class SearchDialog;
}

class SearchDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SearchDialog(QHexEdit *hexEdit, QWidget *parent = 0);
    ~SearchDialog();
    qint64 findNext();
    Ui::SearchDialog *ui;

private slots:
    void on_pbFind_clicked();

private:
    QByteArray getContent(int comboIndex, const QString &input);

    QHexEdit *_hexEdit;
    QByteArray _findBa;
};

#endif // SEARCHDIALOG_H
