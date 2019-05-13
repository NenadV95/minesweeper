#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostLineEdit(new QLineEdit)
    , portLineEdit(new QLineEdit)
    , connectButton(new QPushButton(tr("Connect")))
    , userLineEdit(new QLineEdit)
    , tcpSocket(new QTcpSocket(this))
    , blockSize(0)
{

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel(tr("Server name:"));
    QLabel *userLabel = new QLabel(tr("User Name:"));
    userLabel->setBuddy(userLineEdit);
    hostLabel->setBuddy(hostLineEdit);
    QLabel *portLabel = new QLabel(tr("Server port:"));
    portLabel->setBuddy(portLineEdit);

    statusLabel = new QLabel(tr("Connect to server"));

    connectButton->setDefault(true);
    connectButton->setEnabled(false);

    QPushButton *quitButton = new QPushButton(tr("Quit"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->addWidget(userLabel, 0, 0);
    mainLayout->addWidget(userLineEdit, 0, 1);
    mainLayout->addWidget(hostLabel, 1, 0);
    mainLayout->addWidget(hostLineEdit, 1, 1);
    mainLayout->addWidget(portLabel, 2, 0);
    mainLayout->addWidget(portLineEdit, 2, 1);
    mainLayout->addWidget(statusLabel, 3, 0, 1, 2);
    mainLayout->addWidget(buttonBox, 4, 1, 1, 2);

    setWindowTitle("Connect to server");
    userLineEdit->setFocus();

    in.setDevice(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    connect(hostLineEdit, &QLineEdit::textChanged,
            this, &Client::enableConnection);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableConnection);
    connect(connectButton, &QAbstractButton::clicked,
            this, &Client::requestData);
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(tcpSocket, &QIODevice::readyRead, this, &Client::readData);


}

void Client::requestData()
{
    connectButton->setEnabled(false);
    tcpSocket->abort();

    tcpSocket->connectToHost(hostLineEdit->text(),
                            portLineEdit->text().toInt());


    if(tcpSocket->waitForConnected()) {
        QByteArray block;
        block.append(userLineEdit->text());
        tcpSocket->write(block);
        tcpSocket->waitForBytesWritten();
    }


    // qDebug() << "Connecting...";

}

void Client::readData()
{
    QStringList gameTableInfo;
    QString str;
    int i = 0;
    int j = 0;
    bool mine;
    bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)];
    while(true) {
           if (!blockSize) {
               if (tcpSocket->bytesAvailable() < sizeof(quint16)) {
                   break;
               }
               in >> blockSize;
           }

           if (tcpSocket->bytesAvailable() < blockSize) {
               break;
           }

           if(i < 3) {
               in >> str;

               gameTableInfo.append(str);
           }
           else {
               in >> mine;
               mines[j] = mine;
               j++;
           }
           i++;
           blockSize = tcpSocket->bytesAvailable();
       }

    statusLabel->setText("Connected");
    connectButton->setEnabled(false);

    game.setBoardHeight(gameTableInfo[0].toInt());
    game.setBoardWidth(gameTableInfo[1].toInt());
    game.setBoardNumberOfMines(gameTableInfo[2].toInt());
    game.setServerGame(true);
    game.set(mines, userLineEdit->text());
    game.show();


}

void Client::enableConnection()
{
    connectButton->setEnabled(!hostLineEdit->text().isEmpty() &&
                                 !portLineEdit->text().isEmpty());

}

