#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_textviewer.h"
#include "wombatfunctions.h"

namespace Ui
{
    class TextViewer;
}

class TextViewer : public QDialog
{
    Q_OBJECT
        
public:
    TextViewer(QWidget* parent = 0);
    ~TextViewer();
    void ShowText(const QModelIndex &index);
    
private slots:
    void HideClicked();
signals:
    void HideTextViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::TextViewer* ui;
    void GetTextContent(int objectid);
};

Q_DECLARE_METATYPE(TextViewer*);

#endif // TEXTVIEWER_H
