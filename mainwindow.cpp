#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    menuWidget = new QWidget(this);
    menuLayout = new QVBoxLayout;

    startButton = new QPushButton(tr("Start Game"), this);
    settingsButton = new QPushButton(tr("Settings"), this);
    highscoresButton = new QPushButton(tr("Highscores"), this);
    quitButton = new QPushButton(tr("Quit"), this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setMainMenu()
{
    menuLayout->addWidget(startButton);
    menuLayout->addWidget(settingsButton);
    menuLayout->addWidget(highscoresButton);
    menuLayout->addWidget(quitButton);

    connect(quitButton, SIGNAL(pressed()), this, SLOT(handleQuit()));
    connect(settingsButton, SIGNAL(pressed()), this, SLOT(setupSettings()));
    connect(highscoresButton, SIGNAL(pressed()), this, SLOT(showHighscores()));
    connect(startButton, SIGNAL(pressed()), this, SLOT(handleStart()));


    menuLayout->setSpacing(20);
    menuLayout->setMargin(80);
    menuWidget->setLayout(menuLayout);
    setCentralWidget(menuWidget);


}

void MainWindow::handleStart()
{
    game.setBoardHeight(settings.getHeight());
    game.setBoardWidth(settings.getWidth());
    game.setBoardNumberOfMines(settings.getNumberOfMines());
    game.set();
    game.show();

}

void MainWindow::handleQuit()
{
    QApplication::quit();
}


void MainWindow::on_actionQuit_triggered()
{
    QApplication::quit();
}

void MainWindow::setupSettings()
{
    settings.show();
}

void MainWindow::showHighscores()
{
    h.set("s");
    h.show();
}

