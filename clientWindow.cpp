#include "clientWindow.h"
#include "ui_clientWindow.h"

ClientWindow::ClientWindow(const QString& host_ip, const int& host_port, QWidget *parent)
    : QMainWindow(parent), _hostIp(host_ip),
      _hostPort(host_port), ui(new Ui::MainWindow), _blockSize(0)
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
    this->_socket->disconnectFromHost();

    delete ui;
}


//Buttons slots
void ClientWindow::on_connectButton_clicked()
{
     this->ui->userNameLabel->setStyleSheet("color: rgb(0, 0, 0)");

    //If entered user name
    if(!this->ui->userNameLineEdit->text().isEmpty())
    {
        this->_clientInfo.user_name = this->ui->userNameLineEdit->text();

        //Connect socket to local adress and port
        this->_socket->connectToHost(this->_hostIp, this->_hostPort);

        if(this->_socket->state() == QTcpSocket::ConnectedState)
        {
            this->ui->connectButton->hide();
            this->ui->userNameLineEdit->hide();
            this->ui->userNameLabel->hide();
        }
    }
    else
    {
        this->ui->userNameLabel->setStyleSheet("color: rgb(255, 0, 0)");
    }
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
            QString user_name;

            //Read data from dataStream
            inDataStream >> time >> user_name >> message;

            //Add message to text browser
            this->ui->textBrowser->append(time.toString() + ' ' + user_name + ": " + message);

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

     outDataStream << quint16(0) << QTime::currentTime() << this->_clientInfo.user_name << message;

     outDataStream.device()->seek(0);
     outDataStream << quint16(this->_data.size() - sizeof(quint16));

     //Writing data bytes in socket
     this->_socket->write(this->_data);

     //Clear lineEdit text
     this->ui->lineEdit->clear();
}
