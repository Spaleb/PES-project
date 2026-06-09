/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.15
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *CANStatus;
    QLabel *TCPStatus;
    QLabel *TCPCircle;
    QLabel *label_2;
    QTextEdit *textEdit;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *bedText;
    QLabel *bedStatus;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *distanceText;
    QLabel *distanceValue;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CANStatus = new QLabel(centralwidget);
        CANStatus->setObjectName(QString::fromUtf8("CANStatus"));
        CANStatus->setGeometry(QRect(60, 170, 158, 36));
        QFont font;
        font.setPointSize(15);
        CANStatus->setFont(font);
        TCPStatus = new QLabel(centralwidget);
        TCPStatus->setObjectName(QString::fromUtf8("TCPStatus"));
        TCPStatus->setGeometry(QRect(60, 100, 158, 78));
        TCPStatus->setFont(font);
        TCPCircle = new QLabel(centralwidget);
        TCPCircle->setObjectName(QString::fromUtf8("TCPCircle"));
        TCPCircle->setGeometry(QRect(150, 130, 21, 21));
        TCPCircle->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 10px;"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(150, 180, 21, 21));
        label_2->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 10px;"));
        textEdit = new QTextEdit(centralwidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setGeometry(QRect(190, 10, 421, 61));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(540, 170, 211, 80));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        bedText = new QLabel(horizontalLayoutWidget);
        bedText->setObjectName(QString::fromUtf8("bedText"));
        bedText->setFont(font);

        horizontalLayout->addWidget(bedText);

        bedStatus = new QLabel(horizontalLayoutWidget);
        bedStatus->setObjectName(QString::fromUtf8("bedStatus"));
        bedStatus->setFont(font);

        horizontalLayout->addWidget(bedStatus);

        horizontalLayoutWidget_2 = new QWidget(centralwidget);
        horizontalLayoutWidget_2->setObjectName(QString::fromUtf8("horizontalLayoutWidget_2"));
        horizontalLayoutWidget_2->setGeometry(QRect(540, 280, 160, 80));
        horizontalLayout_2 = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        distanceText = new QLabel(horizontalLayoutWidget_2);
        distanceText->setObjectName(QString::fromUtf8("distanceText"));
        distanceText->setFont(font);

        horizontalLayout_2->addWidget(distanceText);

        distanceValue = new QLabel(horizontalLayoutWidget_2);
        distanceValue->setObjectName(QString::fromUtf8("distanceValue"));
        distanceValue->setFont(font);

        horizontalLayout_2->addWidget(distanceValue);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        CANStatus->setText(QCoreApplication::translate("MainWindow", "CAN:", nullptr));
        TCPStatus->setText(QCoreApplication::translate("MainWindow", "TCP:", nullptr));
        TCPCircle->setText(QString());
        label_2->setText(QString());
        textEdit->setHtml(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:26pt; font-weight:700;\">Kamer 7 dashboard</span></p></body></html>", nullptr));
        bedText->setText(QCoreApplication::translate("MainWindow", "Beddruk:", nullptr));
        bedStatus->setText(QCoreApplication::translate("MainWindow", "inactief", nullptr));
        distanceText->setText(QCoreApplication::translate("MainWindow", "Afstand:", nullptr));
        distanceValue->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
