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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *CANStatus;
    QLabel *TCPStatus;
    QLabel *TCPCircle;
    QLabel *CANCircle;
    QFrame *horizontalFrame;
    QHBoxLayout *horizontalLayout;
    QLabel *bedText;
    QLabel *bedStatus;
    QFrame *horizontalFrame_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *distanceText;
    QLabel *distanceValue;
    QFrame *horizontalFrame_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *tempText;
    QLabel *tempValue;
    QFrame *horizontalFrame_4;
    QHBoxLayout *horizontalLayout_4;
    QLabel *vochtText;
    QLabel *vochtValue;
    QFrame *horizontalFrame_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *ventText;
    QLabel *ventValue;
    QFrame *horizontalFrame_6;
    QHBoxLayout *horizontalLayout_6;
    QLabel *verwText;
    QLabel *verwValue;
    QFrame *horizontalFrame_7;
    QHBoxLayout *horizontalLayout_7;
    QLabel *brandText;
    QLabel *brandValue;
    QLabel *label;
    QFrame *horizontalFrame_8;
    QHBoxLayout *horizontalLayout_8;
    QLabel *valText;
    QLabel *valValue;
    QFrame *horizontalFrame_9;
    QHBoxLayout *horizontalLayout_9;
    QLabel *PlantText;
    QLabel *PlantValue;
    QPushButton *pushButton;
    QLineEdit *matrixInput;
    QLabel *matrixTekst;
    QPushButton *matrixSend;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 669);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        CANStatus = new QLabel(centralwidget);
        CANStatus->setObjectName(QString::fromUtf8("CANStatus"));
        CANStatus->setGeometry(QRect(40, 80, 158, 36));
        QFont font;
        font.setPointSize(15);
        CANStatus->setFont(font);
        TCPStatus = new QLabel(centralwidget);
        TCPStatus->setObjectName(QString::fromUtf8("TCPStatus"));
        TCPStatus->setGeometry(QRect(40, 10, 158, 78));
        TCPStatus->setFont(font);
        TCPCircle = new QLabel(centralwidget);
        TCPCircle->setObjectName(QString::fromUtf8("TCPCircle"));
        TCPCircle->setGeometry(QRect(130, 40, 21, 21));
        TCPCircle->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 10px;"));
        CANCircle = new QLabel(centralwidget);
        CANCircle->setObjectName(QString::fromUtf8("CANCircle"));
        CANCircle->setGeometry(QRect(130, 90, 21, 21));
        CANCircle->setStyleSheet(QString::fromUtf8("background-color: red;\n"
"border-radius: 10px;"));
        horizontalFrame = new QFrame(centralwidget);
        horizontalFrame->setObjectName(QString::fromUtf8("horizontalFrame"));
        horizontalFrame->setGeometry(QRect(430, 130, 351, 80));
        horizontalFrame->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout = new QHBoxLayout(horizontalFrame);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        bedText = new QLabel(horizontalFrame);
        bedText->setObjectName(QString::fromUtf8("bedText"));
        bedText->setFont(font);

        horizontalLayout->addWidget(bedText);

        bedStatus = new QLabel(horizontalFrame);
        bedStatus->setObjectName(QString::fromUtf8("bedStatus"));
        bedStatus->setFont(font);

        horizontalLayout->addWidget(bedStatus);

        horizontalFrame_2 = new QFrame(centralwidget);
        horizontalFrame_2->setObjectName(QString::fromUtf8("horizontalFrame_2"));
        horizontalFrame_2->setGeometry(QRect(430, 220, 351, 80));
        horizontalFrame_2->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_2 = new QHBoxLayout(horizontalFrame_2);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        distanceText = new QLabel(horizontalFrame_2);
        distanceText->setObjectName(QString::fromUtf8("distanceText"));
        distanceText->setFont(font);

        horizontalLayout_2->addWidget(distanceText);

        distanceValue = new QLabel(horizontalFrame_2);
        distanceValue->setObjectName(QString::fromUtf8("distanceValue"));
        distanceValue->setFont(font);

        horizontalLayout_2->addWidget(distanceValue);

        horizontalFrame_3 = new QFrame(centralwidget);
        horizontalFrame_3->setObjectName(QString::fromUtf8("horizontalFrame_3"));
        horizontalFrame_3->setGeometry(QRect(40, 130, 311, 80));
        horizontalFrame_3->setStyleSheet(QString::fromUtf8("background-color: white;\n"
" border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_3 = new QHBoxLayout(horizontalFrame_3);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        tempText = new QLabel(horizontalFrame_3);
        tempText->setObjectName(QString::fromUtf8("tempText"));

        horizontalLayout_3->addWidget(tempText);

        tempValue = new QLabel(horizontalFrame_3);
        tempValue->setObjectName(QString::fromUtf8("tempValue"));

        horizontalLayout_3->addWidget(tempValue);

        horizontalFrame_4 = new QFrame(centralwidget);
        horizontalFrame_4->setObjectName(QString::fromUtf8("horizontalFrame_4"));
        horizontalFrame_4->setGeometry(QRect(40, 220, 311, 80));
        horizontalFrame_4->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_4 = new QHBoxLayout(horizontalFrame_4);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        vochtText = new QLabel(horizontalFrame_4);
        vochtText->setObjectName(QString::fromUtf8("vochtText"));

        horizontalLayout_4->addWidget(vochtText);

        vochtValue = new QLabel(horizontalFrame_4);
        vochtValue->setObjectName(QString::fromUtf8("vochtValue"));

        horizontalLayout_4->addWidget(vochtValue);

        horizontalFrame_5 = new QFrame(centralwidget);
        horizontalFrame_5->setObjectName(QString::fromUtf8("horizontalFrame_5"));
        horizontalFrame_5->setGeometry(QRect(40, 310, 311, 80));
        horizontalFrame_5->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_5 = new QHBoxLayout(horizontalFrame_5);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        ventText = new QLabel(horizontalFrame_5);
        ventText->setObjectName(QString::fromUtf8("ventText"));

        horizontalLayout_5->addWidget(ventText);

        ventValue = new QLabel(horizontalFrame_5);
        ventValue->setObjectName(QString::fromUtf8("ventValue"));

        horizontalLayout_5->addWidget(ventValue);

        horizontalFrame_6 = new QFrame(centralwidget);
        horizontalFrame_6->setObjectName(QString::fromUtf8("horizontalFrame_6"));
        horizontalFrame_6->setGeometry(QRect(40, 400, 311, 80));
        horizontalFrame_6->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_6 = new QHBoxLayout(horizontalFrame_6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        verwText = new QLabel(horizontalFrame_6);
        verwText->setObjectName(QString::fromUtf8("verwText"));

        horizontalLayout_6->addWidget(verwText);

        verwValue = new QLabel(horizontalFrame_6);
        verwValue->setObjectName(QString::fromUtf8("verwValue"));

        horizontalLayout_6->addWidget(verwValue);

        horizontalFrame_7 = new QFrame(centralwidget);
        horizontalFrame_7->setObjectName(QString::fromUtf8("horizontalFrame_7"));
        horizontalFrame_7->setGeometry(QRect(430, 310, 351, 80));
        horizontalFrame_7->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_7 = new QHBoxLayout(horizontalFrame_7);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        brandText = new QLabel(horizontalFrame_7);
        brandText->setObjectName(QString::fromUtf8("brandText"));

        horizontalLayout_7->addWidget(brandText);

        brandValue = new QLabel(horizontalFrame_7);
        brandValue->setObjectName(QString::fromUtf8("brandValue"));

        horizontalLayout_7->addWidget(brandValue);

        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(250, 30, 301, 29));
        QFont font1;
        font1.setPointSize(24);
        font1.setBold(true);
        font1.setItalic(true);
        label->setFont(font1);
        horizontalFrame_8 = new QFrame(centralwidget);
        horizontalFrame_8->setObjectName(QString::fromUtf8("horizontalFrame_8"));
        horizontalFrame_8->setGeometry(QRect(430, 400, 351, 80));
        horizontalFrame_8->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_8 = new QHBoxLayout(horizontalFrame_8);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        valText = new QLabel(horizontalFrame_8);
        valText->setObjectName(QString::fromUtf8("valText"));

        horizontalLayout_8->addWidget(valText);

        valValue = new QLabel(horizontalFrame_8);
        valValue->setObjectName(QString::fromUtf8("valValue"));

        horizontalLayout_8->addWidget(valValue);

        horizontalFrame_9 = new QFrame(centralwidget);
        horizontalFrame_9->setObjectName(QString::fromUtf8("horizontalFrame_9"));
        horizontalFrame_9->setGeometry(QRect(40, 490, 311, 80));
        horizontalFrame_9->setStyleSheet(QString::fromUtf8("background-color: white;\n"
"border: 2px solid #D0D0D0;\n"
"border-radius: 12px;\n"
"padding: 8px;"));
        horizontalLayout_9 = new QHBoxLayout(horizontalFrame_9);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        PlantText = new QLabel(horizontalFrame_9);
        PlantText->setObjectName(QString::fromUtf8("PlantText"));

        horizontalLayout_9->addWidget(PlantText);

        PlantValue = new QLabel(horizontalFrame_9);
        PlantValue->setObjectName(QString::fromUtf8("PlantValue"));

        horizontalLayout_9->addWidget(PlantValue);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(430, 490, 151, 37));
        matrixInput = new QLineEdit(centralwidget);
        matrixInput->setObjectName(QString::fromUtf8("matrixInput"));
        matrixInput->setGeometry(QRect(430, 570, 271, 31));
        matrixTekst = new QLabel(centralwidget);
        matrixTekst->setObjectName(QString::fromUtf8("matrixTekst"));
        matrixTekst->setGeometry(QRect(430, 540, 291, 29));
        matrixSend = new QPushButton(centralwidget);
        matrixSend->setObjectName(QString::fromUtf8("matrixSend"));
        matrixSend->setGeometry(QRect(720, 570, 51, 31));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 34));
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
        CANCircle->setText(QString());
        bedText->setText(QCoreApplication::translate("MainWindow", "Beddruk:          ", nullptr));
        bedStatus->setText(QCoreApplication::translate("MainWindow", "inactief", nullptr));
        distanceText->setText(QCoreApplication::translate("MainWindow", "Afstand:           ", nullptr));
        distanceValue->setText(QCoreApplication::translate("MainWindow", "x", nullptr));
        tempText->setText(QCoreApplication::translate("MainWindow", "Temperatuur:      ", nullptr));
        tempValue->setText(QCoreApplication::translate("MainWindow", "20.00", nullptr));
        vochtText->setText(QCoreApplication::translate("MainWindow", "Luchtvochtigheid", nullptr));
        vochtValue->setText(QCoreApplication::translate("MainWindow", "50.00", nullptr));
        ventText->setText(QCoreApplication::translate("MainWindow", "Ventilatiestand:     ", nullptr));
        ventValue->setText(QCoreApplication::translate("MainWindow", "uit", nullptr));
        verwText->setText(QCoreApplication::translate("MainWindow", "Verwarmingstand:", nullptr));
        verwValue->setText(QCoreApplication::translate("MainWindow", "uit", nullptr));
        brandText->setText(QCoreApplication::translate("MainWindow", "Brandstatus:  ", nullptr));
        brandValue->setText(QCoreApplication::translate("MainWindow", "Geen brand.", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Kamer 7 Dashboard", nullptr));
        valText->setText(QCoreApplication::translate("MainWindow", "Valstatus:           ", nullptr));
        valValue->setText(QCoreApplication::translate("MainWindow", "niet gevallen.", nullptr));
        PlantText->setText(QCoreApplication::translate("MainWindow", "Plant:                      ", nullptr));
        PlantValue->setText(QCoreApplication::translate("MainWindow", "80", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Reset valstatus", nullptr));
        matrixTekst->setText(QCoreApplication::translate("MainWindow", "Vul tekst in voor op de matrix:", nullptr));
        matrixSend->setText(QCoreApplication::translate("MainWindow", "stuur", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
