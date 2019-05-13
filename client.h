#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>
#include "game.h"
#include <QAbstractSocket>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = Q_NULLPTR);

private slots:
    void requestData();
    void readData();
    void enableConnection();

private:
    QLineEdit *hostLineEdit;
    QLineEdit *portLineEdit;
    QLabel *statusLabel;
    QPushButton *connectButton;
    QLineEdit *userLineEdit;

    QTcpSocket *tcpSocket;
    QDataStream in;
    QString textRead;
    Game game;
    quint16 blockSize;
};

#endif
