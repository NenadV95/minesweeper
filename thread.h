#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QTcpSocket>
#include "game.h"
#include "server.h"

class Thread : public QThread
{
    Q_OBJECT

public:
    Thread(int socketDescriptor, const QString &textCopy, int height, int width, int numberOfMines, bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)], QObject *parent);
    void copyMines(bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)], int height, int width);
    void run() override;

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
    QString clientName;
    Game game;
    bool minesThread[(MAX_SIZE-1)*(MAX_SIZE-1)];

};

#endif
