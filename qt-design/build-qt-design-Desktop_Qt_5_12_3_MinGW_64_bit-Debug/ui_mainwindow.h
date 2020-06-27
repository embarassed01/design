/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QGridLayout *gridLayout;
    QLabel *label;
    QTextEdit *inTextEdit;
    QRadioButton *switchForceRBtn;
    QPushButton *chkBtn;
    QTextEdit *resultEdit;
    QLabel *label_3;
    QLabel *chkLabel;
    QLineEdit *inPwdEdit;
    QLabel *label_2;
    QComboBox *typeComboBox;
    QComboBox *deviceComboBox;
    QLineEdit *minLenEdit;
    QLabel *label_4;
    QLineEdit *maxLenEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(561, 682);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 5);

        inTextEdit = new QTextEdit(centralWidget);
        inTextEdit->setObjectName(QString::fromUtf8("inTextEdit"));

        gridLayout->addWidget(inTextEdit, 1, 0, 1, 5);

        switchForceRBtn = new QRadioButton(centralWidget);
        switchForceRBtn->setObjectName(QString::fromUtf8("switchForceRBtn"));

        gridLayout->addWidget(switchForceRBtn, 6, 0, 1, 1);

        chkBtn = new QPushButton(centralWidget);
        chkBtn->setObjectName(QString::fromUtf8("chkBtn"));

        gridLayout->addWidget(chkBtn, 6, 2, 1, 3);

        resultEdit = new QTextEdit(centralWidget);
        resultEdit->setObjectName(QString::fromUtf8("resultEdit"));
        resultEdit->setFrameShape(QFrame::NoFrame);
        resultEdit->setFrameShadow(QFrame::Plain);

        gridLayout->addWidget(resultEdit, 9, 0, 1, 5);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        chkLabel = new QLabel(centralWidget);
        chkLabel->setObjectName(QString::fromUtf8("chkLabel"));

        gridLayout->addWidget(chkLabel, 8, 0, 1, 5);

        inPwdEdit = new QLineEdit(centralWidget);
        inPwdEdit->setObjectName(QString::fromUtf8("inPwdEdit"));

        gridLayout->addWidget(inPwdEdit, 3, 0, 1, 5);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 5);

        typeComboBox = new QComboBox(centralWidget);
        typeComboBox->setObjectName(QString::fromUtf8("typeComboBox"));

        gridLayout->addWidget(typeComboBox, 5, 0, 1, 5);

        deviceComboBox = new QComboBox(centralWidget);
        deviceComboBox->setObjectName(QString::fromUtf8("deviceComboBox"));

        gridLayout->addWidget(deviceComboBox, 6, 1, 1, 1);

        minLenEdit = new QLineEdit(centralWidget);
        minLenEdit->setObjectName(QString::fromUtf8("minLenEdit"));

        gridLayout->addWidget(minLenEdit, 4, 1, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout->addWidget(label_4, 4, 2, 1, 1);

        maxLenEdit = new QLineEdit(centralWidget);
        maxLenEdit->setObjectName(QString::fromUtf8("maxLenEdit"));

        gridLayout->addWidget(maxLenEdit, 4, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 561, 21));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "Hash Input:   (sys encrypted login hash, extracted from encrypted office/pdf/archive, ... ...)", nullptr));
        switchForceRBtn->setText(QApplication::translate("MainWindow", "Btn", nullptr));
        chkBtn->setText(QApplication::translate("MainWindow", "force crack", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "min len:", nullptr));
        chkLabel->setText(QApplication::translate("MainWindow", "waiting for check ... ...", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Corresponding Password for test:", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "max len:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
