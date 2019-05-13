#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenu>
#include "settings.h"
#include "game.h"
#include "highscores.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setMainMenu();

private slots:
    void handleStart();
    void handleQuit();
    void on_actionQuit_triggered();
    void setupSettings();
    void showHighscores();

private:
    Ui::MainWindow *ui;
    QWidget *menuWidget;
    QMenu *menu;
    QVBoxLayout *menuLayout;
    QPushButton *startButton;
    QPushButton *settingsButton;
    QPushButton *highscoresButton;
    QPushButton *quitButton;
    Settings settings;
    Game game;
    Highscores h;


};

#endif // MAINWINDOW_H
