#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <QWidget>
#include <algorithm>
#include <vector>
#include <QDate>
#include <utility>
#include <qfile.h>
#include <QTextStream>
#include <qmessagebox.h>

namespace Ui {
class Highscores;
}

class Highscores : public QWidget
{
    Q_OBJECT

public:
    explicit Highscores(QWidget *parent = 0);
    ~Highscores();
    bool last(int time,QString size);
    void add(int time, QString username, QString size);
    void set(QString size);
private slots:
    void showResults();
private:
    Ui::Highscores *ui;
    std::vector<std::pair<int, QString>> bestResults;
};

#endif // HIGHSCORES_H
