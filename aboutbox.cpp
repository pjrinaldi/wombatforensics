#include "aboutbox.h"

AboutBox::AboutBox(QWidget* parent) : QDialog(parent), ui(new Ui::AboutBox)
{
    ui->setupUi(this);
    QString abouttext = "<h3>About WombatForensics v1.0</h3>";
    abouttext += "<h5>License: GPLv3</h5>";
    abouttext += "<h4>Created by Pasquale J. Rinaldi, Jr.</h4>";
    abouttext += "<4>Copyright &copyright; 2015 Pasquale Rinaldi</h4>";
    abouttext += "<h5>Email: pjrinaldi@gmail.com&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Phone: (865) 206-8130</h5><br/><br/>";
    abouttext += "qt5 lgplv2.1/3 | lfhex gplv2 | treepata glpv3 | sleuthkit gpl/cpl/ibm";
    ui->label->setText(abouttext);
    this->hide();
}

AboutBox::~AboutBox()
{
}

void AboutBox::HideClicked()
{
    this->hide();
}

void AboutBox::ShowAbout()
{
    this->show();
}

void AboutBox::mousePressEvent(QMouseEvent* e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        this->hide();
    }
}

