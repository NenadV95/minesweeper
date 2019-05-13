#include <QtWidgets>
#include <QtNetwork>

#include <stdlib.h>
#include "gamelobby.h"
#include "server.h"

GameLobby::GameLobby(QWidget *parent)
    : QWidget(parent), addedPlayers(0)
{
    playerList = new QListWidget;
    QTextEdit *chatBox = new QTextEdit;
    QLineEdit *messageInput = new QLineEdit;
    statusLabel = new QLabel;
    startButton = new QPushButton(tr("Start Game"));
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    playerList->setMaximumWidth(100);
    chatBox->setReadOnly(true);


    if (!server.listen()) {
        QMessageBox::critical(this, tr("Game Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // trazimo prvu IPv4 adresu koja nije localhost
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            break;
        }
    }
    // ako nismo nasli, koristimo IPv4 localhost
    if (ipAddress.isEmpty())
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Waiting for players to connect...")
                         .arg(ipAddress).arg(server.serverPort()));


    connect(startButton, SIGNAL(clicked()), this, SLOT(handleStart()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(quitButton);

    QVBoxLayout *chatLayout = new QVBoxLayout;
    chatLayout->addWidget(chatBox);
    chatLayout->addWidget(messageInput);


    QHBoxLayout *textFields = new QHBoxLayout;
    textFields->addWidget(playerList);
    textFields->addLayout(chatLayout);

    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(statusLabel, 0, 0);
    gridLayout->addLayout(textFields, 1, 0);
    gridLayout->addLayout(buttonLayout, 2, 0);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updatePlayerList()));
    timer->start(1000);

    setLayout(gridLayout);


}

void GameLobby::handleStart()
{

}

void GameLobby::updatePlayerList()
{
    int i = addedPlayers;
    for(;i<server.numberOfPlayers;i++) {
        playerList->addItem(server.connectedPlayers[i]);
    }
    addedPlayers = i;
}

