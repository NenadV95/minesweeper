#include "server.h"
#include "thread.h"

#include <stdlib.h>

QStringList Server::connectedPlayers;
int Server::numberOfPlayers = 0;

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    game.setBoardHeight(settings.getHeight());
    game.setBoardWidth(settings.getWidth());
    game.setBoardNumberOfMines(settings.getNumberOfMines());
    randomMines(game.getBoardHeight(), game.getBoardWidth(), game.getBoardNumberOfMines());

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Thread *thread = new Thread(socketDescriptor, textCopy, game.getBoardHeight(), game.getBoardWidth(), game.getBoardNumberOfMines(), mines, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();

}

void Server::randomMines(int height, int width, int nom)
{
    srand(time(NULL));
    int generatedMines = 0;

    while ( generatedMines < nom ){

        int minePosition = rand() % (width*height);

            if (mines[minePosition]==false){
                mines[minePosition]=true;
                generatedMines++;
            }
    }

}

void Server::addPlayer(QString client)
{
    connectedPlayers.push_back(client);

}
