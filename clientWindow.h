#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(const QString& host_ip, const int& host_port, QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_lineEdit_returnPressed();

public slots:
    void slotReadyRead();

private:
    //Variables
    QString _hostIp;
    int _hostPort;

    Ui::MainWindow *ui;

    //Size of sending data
    quint16 _blockSize;

    //Socket and byte array with data
    QTcpSocket* _socket;
    QByteArray  _data;

    //Functions
    void sentToServer(QString message);
};
#endif // CLIENTWINDOW_H
