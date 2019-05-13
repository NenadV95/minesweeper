#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);

    connect(ui->SaveSettingsButton, SIGNAL(pressed()), this, SLOT(saveSettings()));
}

Settings::~Settings()
{
    delete ui;
}

void Settings::saveSettings()
{

    if(ui->smallGameButton->isChecked()){

        width=8;
        height=8;
        numberOfMines=10;

    }
    else if(ui->mediumGameButton->isChecked()){

        width=16;
        height=16;
        numberOfMines=40;

    }
    else if(ui->largeGameButton->isChecked()){

        width=30;
        height=16;
        numberOfMines=99;

    }
    else if(ui->CustomGameButton->isChecked()){

        width=ui->WidthEdit->text().toInt();
        height=ui->HeightEdit->text().toInt();
        numberOfMines=ui->NumberOfMinesEdit->text().toInt();
        //ako je uneta vrednost nekog parametra van dozvoljenog opsega, parametre postavljamo na parametre za malu tablu
        if(width<5 || width>50 || height<5 || height>50 || numberOfMines<4 || numberOfMines>2250 || numberOfMines>width*height*9/10){

            width=8;
            height=8;
            numberOfMines=10;

        }

    }
    this->close();

}


int Settings::getWidth()
{

    return width;

}

int Settings::getHeight()
{

    return height;

}

int Settings::getNumberOfMines()
{

    return numberOfMines;

}
