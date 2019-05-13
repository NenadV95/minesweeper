#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QtGui>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QCloseEvent>
#include "board.hpp"
#include "highscores.h"
#include <qinputdialog.h>

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();
    void setBoardHeight(int h);
    void setBoardWidth(int w);
    void setBoardNumberOfMines(int nom);
    void set();
    void set(bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)]);
    int getBoardHeight();
    int getBoardWidth();
    int getBoardNumberOfMines();
    void saveResult();
protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
    void displayTime();
    void hint();
private:
    Ui::Game *ui;
    int boardWidth;
    int boardHeight;
    int boardNumberOfMines;
    int w;
    int h;
    QGraphicsScene scene;
    QGraphicsPixmapItem *fields[MAX_SIZE][MAX_SIZE];
    Board *board;
    QPixmap *mine0;
    QPixmap *mine1;
    QPixmap *mine2;
    QPixmap *mine3;
    QPixmap *mine4;
    QPixmap *mine5;
    QPixmap *mine6;
    QPixmap *mine7;
    QPixmap *mine8;
    QPixmap *mineClosed;
    QPixmap *bomb;
    QPixmap *flag;
    bool finished;
    QTimer timer;
    QTime t;
    int gameTime;
    Highscores highscores;
    void openFields(std::vector<int> result);
    void setWidthAndHeight();
    void loadImages();
};

#endif // GAME_H
