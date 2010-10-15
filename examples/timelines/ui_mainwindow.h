/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri 15. Oct 14:00:16 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QTextEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout_6;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QTextEdit *homeTimelineTextEdit;
    QWidget *tab_2;
    QGridLayout *gridLayout_2;
    QTextEdit *mentionsTextEdit;
    QWidget *tab_3;
    QGridLayout *gridLayout_3;
    QTextEdit *userTimelineTextEdit;
    QWidget *tab_4;
    QGridLayout *gridLayout_4;
    QTextEdit *directMessagesTextEdit;
    QGridLayout *gridLayout_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *usernameLineEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *passwordLineEdit;
    QPushButton *authorizePushButton;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(627, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout_6 = new QGridLayout(centralwidget);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        homeTimelineTextEdit = new QTextEdit(tab);
        homeTimelineTextEdit->setObjectName(QString::fromUtf8("homeTimelineTextEdit"));
        homeTimelineTextEdit->setReadOnly(true);

        gridLayout->addWidget(homeTimelineTextEdit, 0, 0, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        gridLayout_2 = new QGridLayout(tab_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mentionsTextEdit = new QTextEdit(tab_2);
        mentionsTextEdit->setObjectName(QString::fromUtf8("mentionsTextEdit"));
        mentionsTextEdit->setReadOnly(true);

        gridLayout_2->addWidget(mentionsTextEdit, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        gridLayout_3 = new QGridLayout(tab_3);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        userTimelineTextEdit = new QTextEdit(tab_3);
        userTimelineTextEdit->setObjectName(QString::fromUtf8("userTimelineTextEdit"));
        userTimelineTextEdit->setReadOnly(true);

        gridLayout_3->addWidget(userTimelineTextEdit, 0, 0, 1, 1);

        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        gridLayout_4 = new QGridLayout(tab_4);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        directMessagesTextEdit = new QTextEdit(tab_4);
        directMessagesTextEdit->setObjectName(QString::fromUtf8("directMessagesTextEdit"));
        directMessagesTextEdit->setReadOnly(true);

        gridLayout_4->addWidget(directMessagesTextEdit, 0, 0, 1, 1);

        tabWidget->addTab(tab_4, QString());

        gridLayout_6->addWidget(tabWidget, 0, 0, 1, 2);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        usernameLineEdit = new QLineEdit(centralwidget);
        usernameLineEdit->setObjectName(QString::fromUtf8("usernameLineEdit"));

        horizontalLayout->addWidget(usernameLineEdit);


        gridLayout_5->addLayout(horizontalLayout, 0, 0, 1, 2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        passwordLineEdit = new QLineEdit(centralwidget);
        passwordLineEdit->setObjectName(QString::fromUtf8("passwordLineEdit"));
        passwordLineEdit->setEchoMode(QLineEdit::Password);

        horizontalLayout_2->addWidget(passwordLineEdit);


        gridLayout_5->addLayout(horizontalLayout_2, 1, 0, 1, 2);

        authorizePushButton = new QPushButton(centralwidget);
        authorizePushButton->setObjectName(QString::fromUtf8("authorizePushButton"));

        gridLayout_5->addWidget(authorizePushButton, 2, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer, 2, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 1, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(368, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_2, 1, 1, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", " Home timeline", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Mentions timeline", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "User timeline", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("MainWindow", "Direct Messages", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Username:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Password:", 0, QApplication::UnicodeUTF8));
        authorizePushButton->setText(QApplication::translate("MainWindow", "Authorize", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
