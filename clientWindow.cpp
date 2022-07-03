#include "clientWindow.h"
#include "ui_mainwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      _blockSize(0)
{
    ui->setupUi(this);

    //Creating socket
    this->_socket = new QTcpSocket(this);

    //Connecting sockets
    QObject::connect(this->_socket, &QTcpSocket::readyRead, this, &ClientWindow::slotReadyRead);
    QObject::connect(this->_socket, &QTcpSocket::disconnected, this->_socket, &QTcpSocket::deleteLater);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}


//Buttons slots
void ClientWindow::on_connectButton_clicked()
{
    //Connect socket to local adress and port
    this->_socket->connectToHost("127.0.0.1", 2323);
}

//Clicked on button
void ClientWindow::on_sendButton_clicked()
{
    //Sent message on server
    this->sentToServer(this->ui->lineEdit->text());
}

//Clicked ENTER
void ClientWindow::on_lineEdit_returnPressed()
{
    //Sent message on server
    this->sentToServer(this->ui->lineEdit->text());
}


//Reading and saving messages functions

//Reading message from data stream
void ClientWindow::slotReadyRead()
{
    //QDataStream for reading data
    QDataStream inDataStream(this->_socket);

    //QDataStream version
    inDataStream.setVersion(QDataStream::Version::Qt_6_3);

    if(inDataStream.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if(this->_blockSize == 0)
            {
                if(this->_socket->bytesAvailable() < 2) //If no byte available quit
                {
                    break;
                }

                //Reading size of data block
                inDataStream >> this->_blockSize;
            }


            if(this->_socket->bytesAvailable() < this->_blockSize) //Data sending is not complete
            {
                break;
            }

            QString message;
            QTime   time;

            //Read data from dataStream
            inDataStream >> time >> message;

            //Add message to text browser
            this->ui->textBrowser->append(time.toString() + ' ' + message);

            this->_blockSize = 0;
        }
    }
    else
    {
         this->ui->textBrowser->append("Reading error");
    }
}


//Send message on server
void ClientWindow::sentToServer(QString message)
{
    //Clearing data before sending new message
    this->_data.clear();

    //DataStream for sending data
     QDataStream outDataStream(&this->_data, QIODevice::WriteOnly);

     //QDataStream version
     outDataStream.setVersion(QDataStream::Version::Qt_6_3);

     outDataStream << quint16(0) << QTime::currentTime() << message;

     outDataStream.device()->seek(0);
     outDataStream << quint16(this->_data.size() - sizeof(quint16));

     //Writing data bytes in socket
     this->_socket->write(this->_data);

     //Clear lineEdit text
     this->ui->lineEdit->clear();
}
