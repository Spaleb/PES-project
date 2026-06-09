#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QHBoxLayout>

#include <QDebug>
// Socket headers (indien nodig voor Linux CAN)
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

/**
 * @brief Constructer voor een nieuw Main Window:: Main Window object.
 * 
 * @param parent Moet het parant object update (de UI).
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Dit laadt alles uit je mainwindow.ui!

    if (ui->statusbar) {
        ui->statusbar->hide();
    }

    // Initiele status van de bolletjes instellen
    setCanStatus(false);
    setTcpStatus(false);

    // Start de netwerkverbindingen
    setupCAN();
    setupTCP();
}

QByteArray tcpBuffer;

/**
 * @brief Regelt het opzetten van de CAN-verbinding met de microcontrollers.
 * Op het interface can0 kan worden gelezen en geschreven. Het CAN-statusbolletje
 * wordt aangepast aan de hand van de status van de CAN-verbinding.
 * 
 */
void MainWindow::setupCAN()
{
    struct ifreq ifr;
    struct sockaddr_can addr;

    canSocket = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    if (canSocket < 0) {
        qDebug() << "CAN socket error";
        return;
    }

    strcpy(ifr.ifr_name, "can0");
    ioctl(canSocket, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(canSocket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        qDebug() << "CAN bind failed";
        setCanStatus(false);
        return;
    }

    setCanStatus(true);

    notifier = new QSocketNotifier(canSocket, QSocketNotifier::Read, this);
    connect(notifier, &QSocketNotifier::activated, this, &MainWindow::readCanFrame);
}

/**
 * @brief Bevat een switch statement om CAN-frames met een bepaalde header uit te lezen.
 * De functie kan makkelijk uitgebreid worden door een switch-case toe te voegen.
 * 
 */
void MainWindow::readCanFrame()
{
    struct can_frame frame;
    int nbytes = read(canSocket, &frame, sizeof(struct can_frame));

    if (nbytes <= 0)
        return;

    switch (frame.can_id)
    {
        case 0x102: // Afstandssensor
        {
            uint16_t distance = (frame.data[0] << 8) | frame.data[1];

            // updaten van de UI
            ui->distanceValue->setText(QString::number(distance) + " cm");

            break;
        }
        case 0x10: // Het brand is geactiveerd of gedeactiveerd
        {
            if (frame.data[0] == 1){ // Het brandalarm is geactiveerd, de UI zal rood oplichten
                ui->brandValue->setText("Er is brand!");
                ui->horizontalFrame_7->setStyleSheet("background-color: red; border: 2px solid #D0D0D0; border-radius: 12px; padding: 8px;");
            }else if(frame.data[0] == 0){ // Het brandalarm is gedeactiveerd, de UI zal weer "wit" moeten worden
                ui->brandValue->setText("Geen brand.");
                ui->horizontalFrame_7->setStyleSheet("background-color: white; border: 2px solid #D0D0D0; border-radius: 12px; padding: 8px;");
            }
            break;
        }
        case 0x20: // Iemand is gevallen
        {
            if (frame.data[0] == 1){ // Een patiënt is gevallen, de UI zal rood oplichten
                ui->valValue->setText("Patient gevallen!");
                ui->horizontalFrame_8->setStyleSheet("background-color: red; border: 2px solid #D0D0D0; border-radius: 12px; padding: 8px;");
            }else if(frame.data[0] == 0){ // De patiënt is "niet meer" gevallen, de UI zal weer "wit" worden
                ui->valValue->setText("niet gevallen.");
                ui->horizontalFrame_8->setStyleSheet("background-color: white; border: 2px solid #D0D0D0; border-radius: 12px; padding: 8px;");
            }
        }
    }
}
 
/**
 * @brief Regelt het opzetten van de TCP-connectie. het statusbolletje in de UI zal veranderen van kleur
 * afhankelijk van de status van de TCP-connectie.
 * 
 */
void MainWindow::setupTCP()
{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::readTcpData);
    tcpSocket->connectToHost("10.42.0.1", 8081);

    connect(tcpSocket, &QTcpSocket::connected, this, [=]() {
        setTcpStatus(true);
        qDebug() << "TCP UP ";
    });

    connect(tcpSocket, &QTcpSocket::disconnected, this, [=]() {
        setTcpStatus(false);
    });

    connect(tcpSocket, &QTcpSocket::errorOccurred, this, [=]() {
        setTcpStatus(false);
    });
}

/**
 * @brief Splits TCP berichten op met behulp van de : in het bericht.
 * Vervolgens wordt de UI geüpdate met de informatie die is binnengekregen.
 * 
 */
void MainWindow::readTcpData()
{
    // Vullen van de buffer
    tcpBuffer += QString::fromUtf8(tcpSocket->readAll());

    // Splitst de regel op \n als er veel is binnengekomen
    QStringList lines = tcpBuffer.split('\n');

    tcpBuffer = lines.takeLast(); // incomplete stuk bewaren

    for (QString line : lines)
    {
        line = line.trimmed();
        if (line.isEmpty()){
             continue;
        }
        
        // Er wordt telkens alleen gekeken naar het eerste deel van het bericht. Gesplits door de :, het tweede deel
        // waar de variabele staat wordt gebruikt om de UI te updaten.
        if (line.startsWith("BED:"))
        {
            QString state = line.section(':', 1).section(' ', 0, 0);
            ui->bedStatus->setText(state);
        }
        else if (line.startsWith("ID:"))
        {
            QString id = line.section(':', 1).trimmed();
        }else if (line.startsWith("Temperatuur")){
            QString temp = line.section(':', 1).trimmed();
            ui->tempValue->setText(temp);
        }else if (line.startsWith("Luchtvochtigheid")){
            QString vocht = line.section(':', 1).trimmed();
            ui->vochtValue->setText(vocht);
        }else if (line.startsWith("Ventilatiestand")){
            QString vent = line.section(':', 1).trimmed();
            ui->ventValue->setText(vent);
        }else if (line.startsWith("Verwarming")){
            QString verw = line.section(':', 1).trimmed();
            ui->verwValue->setText(verw);
        }else if (line.startsWith("MOI")){
            QString moisture = line.section(':', 1).trimmed();
            ui->PlantValue->setText(moisture);
        }
    }
}

/**
 * @brief Veranderd de kleur van het CAN-status bolletje in de UI. Krijft connected mee voor de status.
 * 
 * @param connected 
 */
void MainWindow::setCanStatus(bool connected)
{
    // Match met de designer: 'CANStatus' bolletje
    if (connected)
        ui->CANCircle->setStyleSheet("background-color: green; border-radius: 10px;");
    else
        ui->CANCircle->setStyleSheet("background-color: red; border-radius: 10px;");
}

/**
 * @brief Veranderd de kleur van het TCP-status bolletje in de UI. Krijft connected mee voor de status.
 * 
 * @param connected 
 */
void MainWindow::setTcpStatus(bool connected)
{
    if (connected)
        ui->TCPCircle->setStyleSheet("background-color: green; border-radius: 10px;");
    else
        ui->TCPCircle->setStyleSheet("background-color: red; border-radius: 10px;");
}

/**
 * @brief Fungeert als reset knop voor de valstatus, de kleur wordt weer teruggezet.
 * 
 */
void MainWindow::on_pushButton_clicked()
{
    ui->valValue->setText("niet gevallen.");
    ui->horizontalFrame_8->setStyleSheet("background-color: white; border: 2px solid #D0D0D0; border-radius: 12px; padding: 8px;");
}

/**
 * @brief Verztuurd de tekst naar de matrix om deze handmatig aan te kunnen passen.
 * 
 */
void MainWindow::on_matrixSend_clicked()
{
    QString matrixString = ui->matrixInput->text();
    tcpSocket->write("MATRIX:" + matrixString.toUtf8() + "\n");
    tcpSocket->flush();
}

/**
 * @brief Vernietigd het Main Window:: Main Window object
 * 
 */
MainWindow::~MainWindow()
{
    delete ui;
}

