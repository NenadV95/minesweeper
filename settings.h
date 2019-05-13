#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();
    int getWidth();
    int getHeight();
    int getNumberOfMines();

private slots:

    void saveSettings();

private:
    Ui::Settings *ui;
    int width=8;
    int height=8;
    int numberOfMines=10;
};

#endif // SETTINGS_H
