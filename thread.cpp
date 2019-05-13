#include "thread.h"
#include <QtNetwork>

Thread::Thread(int socketDescriptor, const QString &textCopy, int height, int width, int numberOfMines, bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)], QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), text(textCopy), clientName("")
{
    game.setBoardHeight(height);
    game.setBoardWidth(width);
    game.setBoardNumberOfMines(numberOfMines);
    copyMines(mines, height, width);
}

void Thread::run()
{
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        emit error(tcpSocket.error());
        return;
    }


    if(tcpSocket.waitForReadyRead(2000)) {
        clientName = QString(tcpSocket.readLine());
        Server::addPlayer(clientName);
        Server::numberOfPlayers++;
    }




    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    // out << text;
    QString height = QString::number(game.getBoardHeight());
    QString width = QString::number(game.getBoardWidth());
    QString numberOfMines = QString::number(game.getBoardNumberOfMines());

    out << quint16(0)
        << height
        << width
        << numberOfMines;

    for(int i=0;i<height.toInt()*width.toInt();i++) {
        out << minesThread[i];

    }


    out.device()->seek(0);
    out << quint16(block.size() - sizeof(quint16));



    // qDebug() << game.getBoardHeight() << game.getBoardWidth() << game.getBoardWidth();

    tcpSocket.write(block);
    tcpSocket.disconnectFromHost();
    tcpSocket.waitForDisconnected();


}

void Thread::copyMines(bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)], int height, int width)
{
    for(int i=0;i<height*width;i++) {
          minesThread[i] = mines[i];

    }

}

