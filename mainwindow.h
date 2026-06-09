 #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QSocketNotifier>

// Voor de CAN structs
#include <linux/can.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief Header file voor de Mainwindow, grotendeels automatisch gegenereerd door
 * QT-creator zelf. 
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readCanFrame();
    void readTcpData();

    void on_pushButton_clicked();

    void on_matrixSend_clicked();

private:
    Ui::MainWindow *ui;

    int canSocket;
    QSocketNotifier *notifier;
    QTcpSocket *tcpSocket;
    QString tcpBuffer;

    void setupCAN();
    void setupTCP();
    void setCanStatus(bool connected);
    void setTcpStatus(bool connected);
};

#endif // MAINWINDOW_H
