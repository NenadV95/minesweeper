#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);

    ui->gameView->setScene(&scene);
    connect(ui->hintButton, SIGNAL(pressed()), this, SLOT(hint()));

}

Game::~Game()
{
    delete ui;
}

//Prilikom klika na Widget, proveravamo da li je pozicija klika u okviru scene, ako jeste odredjujemo koje polje treba otvoriti
void Game::mousePressEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();

    if(y>25){

        x=x/(w/boardWidth)+1;
        y=(y-25)/(h/boardHeight)+1;

        switch (event->button()) {

            case Qt::LeftButton:{

                std::vector<int> result;
                result=board->open(x,y);

                openFields(result);

            }
                break;
            case Qt::RightButton:{

                if(finished==false){

                    int result=board->putFlagOrUnflag(x,y);

                    if(result==1){

                        scene.removeItem(fields[y][x]);
                        fields[y][x]=scene.addPixmap(*flag);
                        fields[y][x]->moveBy(-(w/2)+(x-1)*w/boardWidth,-(h/2)+(y-1)*h/boardHeight);

                    }
                    else if(result==0){

                        scene.removeItem(fields[y][x]);
                        fields[y][x]=scene.addPixmap(*mineClosed);
                        fields[y][x]->moveBy(-(w/2)+(x-1)*w/boardWidth,-(h/2)+(y-1)*h/boardHeight);

                    }

                }

            }
                break;
            default:
                break;
        }
    }
}

//Ako smo uspesno zavrsili partiju proveravamo da li je ta partija medju 10 najboljih tj. najbrzih. Ako jeste, upisujemo rezultat
void Game::saveResult(){

    gameTime=t.elapsed();

    QString size;

    if(board->getWidth()==8 && board->getHeight()==8 && board->getMinesNumber()==10){

        size="s";

    }
    else if(board->getWidth()==16 && board->getHeight()==16 && board->getMinesNumber()==40){

        size="m";

    }
    else if(board->getWidth()==30 && board->getHeight()==16 && board->getMinesNumber()==99){

        size="l";

    }

    if(size=="s" || size=="m" || size=="l"){

        if(highscores.last(gameTime,size)==true){

                QString username=QInputDialog::getText(this, "Enter username", "Username");

                while(username.isEmpty()==true || username.size()>15){

                    username=QInputDialog::getText(this, "Enter username", "Username");

                }

                highscores.add(gameTime,username,size);
                highscores.set(size);
                highscores.show();

        }
    }
}

//Postavljanje velicine prozora, pogleda i svega ostalog u zavisnosti od sirine i visine polja za igru, kao i prikaz svih zatvorenih polja na sceni
void Game::set(){

    scene.clear();

    setWidthAndHeight();

    this->setFixedSize(w+2,h+27);
    ui->gameView->setFixedSize(w+2,h+2);
    ui->gameView->setSceneRect(-(w/2-2),-(h/2-2),w,h);
    ui->timeLabel->setGeometry(this->width()-85,2,80,25);
    ui->hintButton->setGeometry(this->width()/2-35,0,60,25);
    ui->infoLabel->setText("");

    board=new Board(boardWidth, boardHeight, boardNumberOfMines );

    finished=false;

    loadImages();

    for (int i=1 ; i<=boardHeight; i++){
        for (int j=1 ; j<=boardWidth; j++){

            fields[i][j]=scene.addPixmap(*mineClosed);
            fields[i][j]->moveBy(-(w/2)+(j-1)*w/boardWidth,-(h/2)+(i-1)*h/boardHeight);

        }
    }

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    timer.start(1);
    t.start();

}

void Game::set(bool mines[(MAX_SIZE-1)*(MAX_SIZE-1)]){


    setWidthAndHeight();

    this->setFixedSize(w+2,h+27);
    //ui->timeLabel->setGeometry(this->width()-85,2,80,25);
    ui->gameView->setFixedSize(w+2,h+2);

    ui->gameView->setSceneRect(-(w/2-2),-(h/2-2),w,h);
    board=new Board(boardWidth, boardHeight, mines);

    ui->infoLabel->setText("");
    finished=false;

    loadImages();

    for (int i=1 ; i<=boardHeight; i++){
        for (int j=1 ; j<=boardWidth; j++){

            fields[i][j]=scene.addPixmap(*mineClosed);
            fields[i][j]->moveBy(-(w/2)+(j-1)*w/boardWidth,-(h/2)+(i-1)*h/boardHeight);

        }
    }

    QObject::connect(&timer, SIGNAL(timeout()), this, SLOT(displayTime()));
    timer.start(1);
    t.start();


}

//Prikaz vremena partije u labeli. Vreme dobijamo u milisekundama kao rezultat funkcije elapsed, pa ga obradjujemo i ispisujemo kao minute i sekunde
void Game::displayTime(){

    if(finished==false){

        int secs = t.elapsed() / 1000;
        int mins = (secs / 60) % 60;
        secs = secs % 60;
        QString seconds = QString::number(secs);
        QString minutes = QString::number(mins);

        if(mins<10  && secs<10){

            ui->timeLabel->setText("0"+minutes+":"+"0"+seconds);

        }
        else if(mins<10){

            ui->timeLabel->setText("0"+minutes+":"+seconds);

        }
        else if(secs<10){

            ui->timeLabel->setText(minutes+":"+"0"+seconds);

        }
        else{

           ui->timeLabel->setText(minutes+":"+seconds);

        }

    }


}

//podesavanje sirine i visine pogleda i scena u zavisnosti od velicine polja za igru
void Game::setWidthAndHeight(){

    if(boardWidth<12){

        w=400;

    }
    else if(boardWidth<24){

        w=600;

    }
    else{

        w=900;

    }

    if(boardHeight<12){

        h=300;

    }
    else if(boardHeight<24){

        h=450;

    }
    else{

        h=700;

    }

    if(boardWidth>boardHeight*3.5){

        h=h*0.8;

    }

    if(boardHeight>boardWidth*3.5){

        w=w*0.6;

    }

}

void Game::loadImages(){

    mine0=new QPixmap(QPixmap(":/images/0.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine1=new QPixmap(QPixmap(":/images/1.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine2=new QPixmap(QPixmap(":/images/2.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine3=new QPixmap(QPixmap(":/images/3.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine4=new QPixmap(QPixmap(":/images/4.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine5=new QPixmap(QPixmap(":/images/5.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine6=new QPixmap(QPixmap(":/images/6.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine7=new QPixmap(QPixmap(":/images/7.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mine8=new QPixmap(QPixmap(":/images/8.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    mineClosed=new QPixmap(QPixmap(":/images/closed.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    bomb=new QPixmap(QPixmap(":/images/bomb.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));
    flag=new QPixmap(QPixmap(":/images/flag.png").scaled(w/boardWidth, h/boardHeight, Qt::IgnoreAspectRatio,
                                         Qt::FastTransformation));

}

//Otvaranje polja zadatih u nizu. Niz sadrzi x  y koordinate, kao i broj susednih polja, na osnovu koga treba iscrtati odredjenu sliku
//Ako je clan 3*i+2 jednak -1 znaci da smo pogresili i treba iscrtati bombu. Ako niz ima 3*i+1 elemenata, znaci da smo uspesno zavrsili partiju
void Game::openFields(std::vector<int> result){

    unsigned int n=result.size()/3;

    if(finished==false){

        for(unsigned int i=0;i<n;i++){

            if(result[3*i+2]==-1){

                finished=true;
                scene.removeItem(fields[result[3*i+1]][result[3*i]]);
                fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*bomb);
                fields[result[3*i+1]][result[3*i]]->moveBy(-(w/2)+(result[3*i]-1)*w/boardWidth,-(h/2)+(result[3*i+1]-1)*h/boardHeight);

            }
            else{

                scene.removeItem(fields[result[3*i+1]][result[3*i]]);
                if(result[3*i+2]==0){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine0);

                }
                else if(result[3*i+2]==1){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine1);

                }
                else if(result[3*i+2]==2){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine2);

                }
                else if(result[3*i+2]==3){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine3);

                }
                else if(result[3*i+2]==4){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine4);

                }
                else if(result[3*i+2]==5){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine5);

                }
                else if(result[3*i+2]==6){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine6);

                }
                else if(result[3*i+2]==7){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine7);

                }
                else if(result[3*i+2]==8){

                    fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*mine8);

                }

                fields[result[3*i+1]][result[3*i]]->moveBy(-(w/2)+(result[3*i]-1)*w/boardWidth,-(h/2)+(result[3*i+1]-1)*h/boardHeight);

            }
        }
        if(finished==true){

            ui->infoLabel->setText("Game Over!");

        }
        if(result.size()%3!=0 && result.size()>1){

            finished=true;
            ui->infoLabel->setText("You won!");
            saveResult();

        }
    }


}

//Slot koji prilikom pritiska na dugme hintButton poziva metode klase board hintFlag i hintOpen.
//Ako funkcija hintFlag ne uspe da pronadje nijedno polje koje treba flagovati, onda se poziva hintOpen, a ako ni on ne pronadje polje koje treba otvoriti, onda ste prepusteni sami sebi
void Game::hint(){

    std::vector<int> result;
    result=board->hintFlag();

    if(result.size()!=0){

        unsigned int n=result.size()/3;

        if(finished==false){

            for(unsigned int i=0;i<n;i++){

                scene.removeItem(fields[result[3*i+1]][result[3*i]]);
                fields[result[3*i+1]][result[3*i]]=scene.addPixmap(*flag);
                fields[result[3*i+1]][result[3*i]]->moveBy(-(w/2)+(result[3*i]-1)*w/boardWidth,-(h/2)+(result[3*i+1]-1)*h/boardHeight);

            }
        }
    }
    else{

        result=board->hintOpen();
        openFields(result);

    }
}

int Game::getBoardHeight(){

    return boardHeight;
}

int Game::getBoardWidth(){

    return boardWidth;
}

int Game::getBoardNumberOfMines(){

    return boardNumberOfMines;
}

void Game::setBoardHeight(int h){

    boardHeight=h;

}

void Game::setBoardWidth(int w){

    boardWidth=w;

}

void Game::setBoardNumberOfMines(int nom){

    boardNumberOfMines=nom;

}
