#include "fileviewer.h"

FileViewer::FileViewer(QWidget* parent) : QMainWindow(parent), ui(new Ui::FileViewer)
{
    ui->setupUi(this);
}

FileViewer::~FileViewer()
{
    this->close();
}
