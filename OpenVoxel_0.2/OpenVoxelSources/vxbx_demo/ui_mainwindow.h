/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Jun 11 01:30:42 2012
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QLocale>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QFrame *sidebar;
    QVBoxLayout *verticalLayout;
    QLabel *label_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QFrame *frame;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *rbtn_float;
    QFrame *frame_2;
    QGridLayout *gridLayout_3;
    QCheckBox *check_fullscreen;
    QPushButton *btn_reloadshader;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_3;
    QLineEdit *edit_re;
    QLineEdit *edit_im;
    QLabel *label_4;
    QLineEdit *edit_zoom;
    QLabel *label_6;
    QLineEdit *edit_iterations;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_2;
    QCheckBox *check_Benchmark;
    QLabel *label_2;
    QLabel *lab_fps;
    QSpacerItem *verticalSpacer;
    QFrame *centralFrame;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *GLContainer;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(630, 420);
        MainWindow->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sidebar = new QFrame(centralWidget);
        sidebar->setObjectName(QString::fromUtf8("sidebar"));
        sidebar->setFrameShape(QFrame::StyledPanel);
        sidebar->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(sidebar);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        label_5 = new QLabel(sidebar);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setAlignment(Qt::AlignCenter);
        label_5->setWordWrap(true);

        verticalLayout->addWidget(label_5);

        groupBox = new QGroupBox(sidebar);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame);
        verticalLayout_3->setSpacing(4);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(-1, 0, 0, 9);
        rbtn_float = new QRadioButton(frame);
        rbtn_float->setObjectName(QString::fromUtf8("rbtn_float"));
        rbtn_float->setEnabled(false);
        rbtn_float->setChecked(true);

        verticalLayout_3->addWidget(rbtn_float);


        verticalLayout_2->addWidget(frame);

        frame_2 = new QFrame(groupBox);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(9, 0, 9, 4);
        check_fullscreen = new QCheckBox(frame_2);
        check_fullscreen->setObjectName(QString::fromUtf8("check_fullscreen"));

        gridLayout_3->addWidget(check_fullscreen, 0, 0, 1, 1);

        btn_reloadshader = new QPushButton(frame_2);
        btn_reloadshader->setObjectName(QString::fromUtf8("btn_reloadshader"));

        gridLayout_3->addWidget(btn_reloadshader, 0, 1, 1, 1);


        verticalLayout_2->addWidget(frame_2);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(sidebar);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(groupBox_2);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        edit_re = new QLineEdit(groupBox_2);
        edit_re->setObjectName(QString::fromUtf8("edit_re"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(edit_re->sizePolicy().hasHeightForWidth());
        edit_re->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edit_re, 0, 1, 1, 1);

        edit_im = new QLineEdit(groupBox_2);
        edit_im->setObjectName(QString::fromUtf8("edit_im"));
        sizePolicy.setHeightForWidth(edit_im->sizePolicy().hasHeightForWidth());
        edit_im->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edit_im, 2, 1, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        edit_zoom = new QLineEdit(groupBox_2);
        edit_zoom->setObjectName(QString::fromUtf8("edit_zoom"));
        sizePolicy.setHeightForWidth(edit_zoom->sizePolicy().hasHeightForWidth());
        edit_zoom->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edit_zoom, 3, 1, 1, 1);

        label_6 = new QLabel(groupBox_2);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        edit_iterations = new QLineEdit(groupBox_2);
        edit_iterations->setObjectName(QString::fromUtf8("edit_iterations"));
        sizePolicy.setHeightForWidth(edit_iterations->sizePolicy().hasHeightForWidth());
        edit_iterations->setSizePolicy(sizePolicy);

        gridLayout->addWidget(edit_iterations, 4, 1, 1, 1);


        verticalLayout->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(sidebar);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_2 = new QGridLayout(groupBox_3);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        check_Benchmark = new QCheckBox(groupBox_3);
        check_Benchmark->setObjectName(QString::fromUtf8("check_Benchmark"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(check_Benchmark->sizePolicy().hasHeightForWidth());
        check_Benchmark->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(check_Benchmark, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        lab_fps = new QLabel(groupBox_3);
        lab_fps->setObjectName(QString::fromUtf8("lab_fps"));
        sizePolicy2.setHeightForWidth(lab_fps->sizePolicy().hasHeightForWidth());
        lab_fps->setSizePolicy(sizePolicy2);

        gridLayout_2->addWidget(lab_fps, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addWidget(sidebar);

        centralFrame = new QFrame(centralWidget);
        centralFrame->setObjectName(QString::fromUtf8("centralFrame"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(centralFrame->sizePolicy().hasHeightForWidth());
        centralFrame->setSizePolicy(sizePolicy3);
        centralFrame->setFrameShape(QFrame::Box);
        centralFrame->setFrameShadow(QFrame::Plain);
        centralFrame->setLineWidth(1);
        horizontalLayout_2 = new QHBoxLayout(centralFrame);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        GLContainer = new QGridLayout();
        GLContainer->setSpacing(0);
        GLContainer->setObjectName(QString::fromUtf8("GLContainer"));

        horizontalLayout_2->addLayout(GLContainer);


        horizontalLayout->addWidget(centralFrame);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 630, 21));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GLSL Mandelbot Demo", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">GLSL Mandelbrot Demo Application</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Drag with left mouse button.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Zoom with mouse wheel.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-righ"
                        "t:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Confirm changed values with 'Enter'.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Render Mode", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        rbtn_float->setToolTip(QApplication::translate("MainWindow", "Use single precision (float32) for fractal rendering.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        rbtn_float->setText(QApplication::translate("MainWindow", "single precision", 0, QApplication::UnicodeUTF8));
        check_fullscreen->setText(QApplication::translate("MainWindow", "Fullscreen", 0, QApplication::UnicodeUTF8));
        btn_reloadshader->setText(QApplication::translate("MainWindow", "reload Shader", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Fractal Settings", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Re:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Im:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Zoom:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Iterations:", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Benchmark", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        check_Benchmark->setToolTip(QApplication::translate("MainWindow", "WARNING: Benchmark mode might stress your GPU.", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        check_Benchmark->setText(QApplication::translate("MainWindow", "enable", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Framerate:", 0, QApplication::UnicodeUTF8));
        lab_fps->setText(QApplication::translate("MainWindow", "-", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
