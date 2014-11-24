#ifndef FILEVIEWER_H
#define FILEVIEWER_H

#include "wombatinclude.h"
#include "ui_fileviewer.h"

namespace Ui {
    class FileViewer;
}
    
class FileViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    FileViewer(QWidget* parent = 0);
    ~FileViewer();

private:
    Ui::FileViewer* ui;
};

Q_DECLARE_METATYPE(FileViewer*);

#endif // FILEVIWER_H
