#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H

#include "wombatinclude.h"
#include "globals.h"
#include "ui_textviewer.h"

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
    
private slots:
    void HideClicked();
signals:
    void HideTextViewerWindow(bool checkstate);

protected:
    void closeEvent(QCloseEvent* event);

private:
    Ui::TextViewer* ui;
};

Q_DECLARE_METATYPE(TextViewer*);

#endif // TEXTVIEWER_H
