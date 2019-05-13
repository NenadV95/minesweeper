#ifndef GAMELOBBY_H
#define GAMELOBBY_H

#include <QWidget>
#include "server.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QListWidget;
QT_END_NAMESPACE

class GameLobby : public QWidget
{
    Q_OBJECT

public:
    GameLobby(QWidget *parent = 0);

private slots:
    void handleStart();
    void updatePlayerList();


private:
    QLabel *statusLabel;
    QPushButton *startButton;
    QPushButton *quitButton;
    Server server;
    QListWidget *playerList;
    int addedPlayers;

};

#endif
