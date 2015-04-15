/********************************************************************************
** Form generated from reading UI file 'videoviewer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VIDEOVIEWER_H
#define UI_VIDEOVIEWER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_VideoViewer
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QFrame *frame;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *label2;
    QLabel *label3;
    QSlider *horizontalSlider;
    QPushButton *pushButton;

    void setupUi(QDialog *VideoViewer)
    {
        if (VideoViewer->objectName().isEmpty())
            VideoViewer->setObjectName(QStringLiteral("VideoViewer"));
        VideoViewer->resize(660, 398);
        verticalLayout = new QVBoxLayout(VideoViewer);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        label_2 = new QLabel(VideoViewer);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_2);


        verticalLayout->addLayout(horizontalLayout);

        frame = new QFrame(VideoViewer);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 0));
        frame->setMaximumSize(QSize(16777215, 20));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        horizontalLayout_2 = new QHBoxLayout(frame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        label2 = new QLabel(frame);
        label2->setObjectName(QStringLiteral("label2"));

        horizontalLayout_2->addWidget(label2);

        label3 = new QLabel(frame);
        label3->setObjectName(QStringLiteral("label3"));

        horizontalLayout_2->addWidget(label3);

        horizontalSlider = new QSlider(frame);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(horizontalSlider);

        pushButton = new QPushButton(frame);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_2->addWidget(pushButton);


        verticalLayout->addWidget(frame);


        retranslateUi(VideoViewer);

        QMetaObject::connectSlotsByName(VideoViewer);
    } // setupUi

    void retranslateUi(QDialog *VideoViewer)
    {
        VideoViewer->setWindowTitle(QApplication::translate("VideoViewer", "View Video", 0));
        label_2->setText(QApplication::translate("VideoViewer", "Loading...", 0));
        label->setText(QApplication::translate("VideoViewer", "00:00:00", 0));
        label2->setText(QApplication::translate("VideoViewer", "/", 0));
        label3->setText(QApplication::translate("VideoViewer", "00:00:00", 0));
        pushButton->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VideoViewer: public Ui_VideoViewer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIDEOVIEWER_H
