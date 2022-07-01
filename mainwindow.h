#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_lineEdit_returnPressed();

public slots:
    void slotReadyRead();

private:
    //Variables
    Ui::MainWindow *ui;

    //Size of sending data
    quint16 _blockSize;

    //Socket and byte array with data
    QTcpSocket* _socket;
    QByteArray  _data;

    //Functions
    void sentToServer(QString message);
};
#endif // MAINWINDOW_H
