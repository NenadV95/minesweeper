#include "highscores.h"
#include "ui_highscores.h"

Highscores::Highscores(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Highscores)
{
    ui->setupUi(this);
    connect(ui->smallButton, SIGNAL(toggled(bool)), this, SLOT(showResults()));
    connect(ui->mediumButton, SIGNAL(toggled(bool)), this, SLOT(showResults()));
    connect(ui->largeButton, SIGNAL(toggled(bool)), this, SLOT(showResults()));

}

Highscores::~Highscores()
{
    delete ui;
}

//Provera da li je uneto vreme bolje od desetog najboljeg, ili ako ih ima manje, onda od poslednjeg. Vremena se cuvaju u txt fajlu zajedno sa nick-ovima
bool Highscores::last(int time, QString size){

    QFile file(size.append(".txt"));

    file.open(QIODevice::ReadOnly | QIODevice::WriteOnly);

    QTextStream t(&file);

    QString s1,s2;
    int time1;

    while(!t.atEnd()){

        s1=t.readLine(30);
        time1=s1.toInt();
        s2=t.readLine(30);
        bestResults.push_back(std::make_pair(time1, s2));

    }

    file.close();

    if(bestResults.size()<10 || time < std::get<0>(bestResults.back())){

        bestResults.clear();
        return true;

    }
    else{

        bestResults.clear();
        return false;

    }


}

//Posto smo prethodno proverili da li smo medju najboljima, dodajemo nas rezultat. Rezultati se cuvaju sortirani u txt fajlu, tako da prilikom upisa, ako ima manje od 10 rezultata u fajlu,
//samo dodajemo nas rezultat i sortiramo, ako ima tacno 10, onda skidamo poslednji, dodajemo nas i potom sortiramo
void Highscores::add(int time, QString username, QString size){

    QFile file(size.append(".txt"));

    file.open(QIODevice::ReadOnly);

    QTextStream t(&file);

    QString s1,s2;
    int time1;

    while(!t.atEnd()){

        s1=t.readLine(30);
        time1=s1.toInt();
        s2=t.readLine(30);
        bestResults.push_back(std::make_pair(time1, s2));

    }


    file.close();

    if(bestResults.size()<10){

        bestResults.push_back(std::make_pair(time, username));
        std::sort(bestResults.begin(), bestResults.end(), [](const std::pair<int, QString> &left, const std::pair<int, QString> &right) {
            return left.first < right.first;
        });

    }
    else{

        bestResults.pop_back();
        bestResults.push_back(std::make_pair(time, username));
        std::sort(bestResults.begin(), bestResults.end(), [](const std::pair<int, QString> &left, const std::pair<int, QString> &right) {
            return left.first < right.first;
        });

    }

    QFile::remove(size);

    file.open(QIODevice::Truncate | QIODevice::Append | QIODevice::WriteOnly);

    unsigned int n=bestResults.size();

    QTextStream t1(&file);

    for(unsigned int i=0;i<n;i++){

        t1 << QString::number(std::get<0>(bestResults[i])) << endl;
        t1 << std::get<1>(bestResults[i]) << endl;

    }

    bestResults.clear();
    file.close();

}

//Ispis rezultata i nick-ova u ListWidget
void Highscores::set(QString size){

    ui->timeWidget->clear();
    ui->usernameWidget->clear();

    QFile file(size.append(".txt"));

    file.open(QIODevice::ReadOnly | QIODevice::WriteOnly);

    QTextStream t(&file);

    QString s1,s2;
    int time1;

    while(!t.atEnd()){

        s1=t.readLine(30);
        time1=s1.toInt();
        s2=t.readLine(30);
        bestResults.push_back(std::make_pair(time1, s2));

    }

    unsigned int n=bestResults.size();

    for(unsigned int i=0;i<n;i++){

        int secs = std::get<0>(bestResults[i]) / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        QString seconds = QString::number(secs);
        QString minutes = QString::number(mins);

        if(mins<10  && secs<10){

            ui->timeWidget->addItem("0"+minutes+":"+"0"+seconds);

        }
        else if(mins<10){

            ui->timeWidget->addItem("0"+minutes+":"+seconds);

        }
        else if(secs<10){

            ui->timeWidget->addItem(minutes+":"+"0"+seconds);

        }
        else{

           ui->timeWidget->addItem(minutes+":"+seconds);

        }
        ui->usernameWidget->addItem(std::get<1>(bestResults[i]));

    }

    bestResults.clear();
    file.close();

}

void Highscores::showResults(){

    ui->usernameWidget->clear();
    ui->timeWidget->clear();

    if(ui->smallButton->isChecked()){

        set("s");

    }
    else if(ui->mediumButton->isChecked()){

        set("m");

    }
    else if(ui->largeButton->isChecked()){

        set("l");

    }


}
