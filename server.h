#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "game.h"
#include "settings.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    void randomMines(int height, int width, int nom);
    static int numberOfPlayers;
    static QStringList connectedPlayers;
    static void addPlayer(QString client);

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    QString textCopy;
    Settings settings;
    Game game;
    bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)];
};

#endif
