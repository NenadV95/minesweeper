#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "board.hpp"

Board::Board(){


}
   
Board::Board(int w,int h,int n_m): width(w),height(h), mineCount(n_m),flaggedCount(0),unknownCount(w*h){

    //inicijalizujemo sva polja na false, tj. da nemaju minu. Matricu smo okruzi;li sa jednim redom ispunjenim false
    //vrednostima kako bi lakse racunali broj susednih mina
    for (int i=0 ; i<=height+1; i++){
        for (int j=0 ; j<=width+1; j++){

            matrixOfFields[i][j].setHasMine(false);
            matrixOfFields[i][j].setFlagged(false);
            matrixOfFields[i][j].setOpened(false);

        }
    }

    //postavljamo mine na random pozicije
    srand(time(NULL));
    int generatedMines = 0;

    while ( generatedMines < mineCount ){

        int minePosition = rand() % (width*height);
        int minePositionX=minePosition%width+1;
        int minePositionY=minePosition/width+1;

            if (matrixOfFields[minePositionY][minePositionX].getHasMine()==false){
                matrixOfFields[minePositionY][minePositionX].setHasMine(true);
                generatedMines++;
            }
    }

    //racunanje broja susednih mina za svako polje
    int nMines=0;
    for (int i=1 ; i<=height; i++){
        for (int j=1 ; j<=width; j++){

                for (int k=-1 ; k<2; k++){
                    for (int l=-1 ; l<2; l++){

                        if(k==0 && l==0){

                        }
                        else{
                            if(matrixOfFields[i+k][j+l].getHasMine()==true)
                                nMines++;
                        }
                    }
                }

                matrixOfFields[i][j].setNumberOfMines(nMines);
                nMines=0;

         }
    }
}

Board::Board(int w, int h, bool mines[(MAX_SIZE-2)*(MAX_SIZE-2)]): width(w),height(h), flaggedCount(0),unknownCount(w*h){

    int counter=0;

    for (int i=0 ; i<=height+1; i++){
        for (int j=0 ; j<=width+1; j++){

            if(i==0 || i==height+1 || j==0 || j==width+1){

                matrixOfFields[i][j].setHasMine(false);

            }
            else{

                if(mines[(i-1)*w+(j-1)]==true){

                    counter++;

                }
                matrixOfFields[i][j].setHasMine(mines[(i-1)*w+(j-1)]);

            }

        }
    }

    //racunanje broja susednih mina za svako polje
    int nMines=0;

    for (int i=1 ; i<=height; i++){
        for (int j=1 ; j<=width; j++){

                for (int k=-1 ; k<2; k++){
                    for (int l=-1 ; l<2; l++){

                        if(k==0 && l==0){

                        }
                        else{
                            if(matrixOfFields[i+k][j+l].getHasMine()==true){

                                nMines++;

                            }
                        }
                    }
                }

                matrixOfFields[i][j].setNumberOfMines(nMines);
                nMines=0;

         }
    }

    mineCount=counter;

}


//kada korisnik klikne na odredjeno polje, pozvace se funkcija open koja za argumente ima x i y koordinate polja.
//Ovo je rekurzivna funkcija, ako korisnik klikne na polje koje nema susednih bombi, funkcija ce se pozvati rekurzivno i za sva susedna polja.
//Takodje, ako korisnik pritisne desni klik, a broj flagovanih susednih polja i broj susednih mina je jednak, otvorice se preostala susedna polja.
std::vector<int> Board::open(int xCoord, int yCoord){

    std::vector<int> result;
    std::vector<int> r;

    if(matrixOfFields[yCoord][xCoord].getFlagged()==false){

        if(matrixOfFields[yCoord][xCoord].getOpened()==true){

            if(matrixOfFields[yCoord][xCoord].getNumberOfMines()==numberOfFlaggedAdjacentMines(xCoord, yCoord)){

                for (int k=-1 ; k<2; k++){
                    for (int l=-1 ; l<2; l++){

                        if(k==0 && l==0){

                        }
                        else{

                            if(matrixOfFields[yCoord+l][xCoord+k].getFlagged()==false && matrixOfFields[yCoord+l][xCoord+k].getOpened()==false){

                                if(xCoord+k<width+1 && xCoord+k>0 && yCoord+l<height+1  && yCoord+l>0){

                                    r=open(xCoord+k,yCoord+l);
                                    result.insert(result.end(),r.begin(),r.end());

                                }

                            }
                        }

                    }
                }

                return result;

            }

        }
        else{
            if (matrixOfFields[yCoord][xCoord].getHasMine()==true){

                result.push_back(xCoord);
                result.push_back(yCoord);
                result.push_back(-1);

                for (int i=1 ; i<=height; i++){
                    for (int j=1 ; j<=width; j++){

                        if(i==yCoord && j==xCoord){

                        }
                        else{

                            if(matrixOfFields[i][j].getHasMine()==true){

                                result.push_back(j);
                                result.push_back(i);
                                result.push_back(-1);

                            }

                        }

                    }
                }
                return result;

            }
            else{

                if(matrixOfFields[yCoord][xCoord].getNumberOfMines()!=0){

                    matrixOfFields[yCoord][xCoord].setOpened(true);
                    unknownCount--;
                    result.push_back(xCoord);
                    result.push_back(yCoord);
                    result.push_back(matrixOfFields[yCoord][xCoord].getNumberOfMines());

                    if(unknownCount==mineCount){

                        result.push_back(1);

                    }

                    return result;

                }
                else{

                    matrixOfFields[yCoord][xCoord].setOpened(true);
                    unknownCount--;
                    result.push_back(xCoord);
                    result.push_back(yCoord);
                    result.push_back(matrixOfFields[yCoord][xCoord].getNumberOfMines());

                    for (int k=-1 ; k<2; k++){
                        for (int l=-1 ; l<2; l++){

                            if(k==0 && l==0){

                            }
                            else{
                                if(matrixOfFields[yCoord+l][xCoord+k].getFlagged()==false && matrixOfFields[yCoord+l][xCoord+k].getOpened()==false){

                                    if(xCoord+k<width+1 && xCoord+k>0 && yCoord+l<height+1  && yCoord+l>0){
                                        r=open(xCoord+k,yCoord+l);
                                        result.insert(result.end(),r.begin(),r.end());
                                    }

                                }
                            }

                        }
                    }

                    if(unknownCount==mineCount){

                        result.push_back(1);

                    }

                    return result;

                }
            }
        }
    }

    result.push_back(-1);
    return result;

}

//Funkcija koja sama otvara polja, tako sto proverava da li je broj mina u susednim poljima jednak broj flagovanih suseda,
//ako jeste, otvorice preostala susedna polja
std::vector<int> Board::hintOpen(){

    std::vector<int> result;
    std::vector<int> r;

    for (int i=1 ; i<=height; i++){
        for (int j=1 ; j<=width; j++){

            int noc=numberOfClosedAdjacentFields(j,i);
            int nom=numberOfAdjacentMines(j,i);
            int f=numberOfFlaggedAdjacentMines(j,i);

            if(nom==f && noc>f && nom!=0 && matrixOfFields[i][j].getFlagged()==false && matrixOfFields[i][j].getOpened()==true){

                for (int k=-1 ; k<2; k++){
                    for (int l=-1 ; l<2; l++){

                        if(k==0 && l==0){

                        }
                        else if(i+k!=0 && i+k!=height+1 && j+l!=0 && j+l!=width+1){

                            if(matrixOfFields[i+k][j+l].getFlagged()==false && matrixOfFields[i+k][j+l].getOpened()==false){

                                r=open(j+l,i+k);
                                result.insert(result.end(),r.begin(),r.end());

                            }
                        }
                    }
                }

                return result;

            }
        }
    }

    return result;

}

//Funkcija koja vraca kao rezultat niz polja koja treba flagovati. To radi tako sto proveri da li je broj neotvorenih polja jednak broj mina u susednim poljima
std::vector<int> Board::hintFlag(){

    std::vector<int> result;

    for (int i=1 ; i<=height; i++){
        for (int j=1 ; j<=width; j++){

            int noc=numberOfClosedAdjacentFields(j,i);
            int nom=numberOfAdjacentMines(j,i);
            int f=numberOfFlaggedAdjacentMines(j,i);

            if(noc==nom && noc!=0 && noc!=f && matrixOfFields[i][j].getOpened()==true){

                for (int k=-1 ; k<2; k++){
                    for (int l=-1 ; l<2; l++){

                        if(k==0 && l==0){

                        }
                        else if(j+k!=0 && j+k!=width+1 && i+l!=0 && i+l!=height+1){

                            if(flagged(j+k,i+l)!=-1 && flagged(j+k,i+l)!=0){

                                result.push_back(j+k);
                                result.push_back(i+l);
                                result.push_back(1);
                                putFlagOrUnflag(j+k,i+l);

                            }
                        }
                    }
                }

                return result;

            }
        }
    }
    return result;
}

//Postavljanje ili skidanje zastavice sa odredjenog polja
int Board::putFlagOrUnflag(int xCoord, int yCoord){

    if(matrixOfFields[yCoord][xCoord].getOpened()==false){

        if (matrixOfFields[yCoord][xCoord].getFlagged()==true){

            matrixOfFields[yCoord][xCoord].setFlagged(false);
            flaggedCount--;
            return 0;

        }
        else{

            matrixOfFields[yCoord][xCoord].setFlagged(true);
            flaggedCount++;
            return 1;

        }
    }
    else{

        return -1;

    }    
}

//Provera da li neko polje ima zastavicu. Za razliku od prethodne metode, ne menja vrednost flagged
int Board::flagged(int xCoord,int yCoord){

    if(matrixOfFields[yCoord][xCoord].getOpened()==false){

        if (matrixOfFields[yCoord][xCoord].getFlagged()==true){

            return 0;

        }
        else{

            return 1;

        }
    }
    else{

        return -1;

    }

}

//Racunanje broja susednih flagovanih polja
int Board::numberOfFlaggedAdjacentMines(int xCoord, int yCoord){

    int numberOfFlagged=0;

    for (int i=-1 ; i<2; i++){
        for (int j=-1 ; j<2; j++){
            if(i==0 && j==0){

            }
            else{
                if(matrixOfFields[yCoord+j][xCoord+i].getFlagged()==true){

                    numberOfFlagged++;

                }
            }

        }

    }

    return numberOfFlagged;

}

int Board::numberOfAdjacentMines(int xCoord, int yCoord){

    return matrixOfFields[yCoord][xCoord].getNumberOfMines();

}

//Vraca broj neotvorenih susednih polja
int Board::numberOfClosedAdjacentFields(int xCoord, int yCoord){

    int numberOfClosed=0;

    for (int i=-1 ; i<2; i++){
        for (int j=-1 ; j<2; j++){

            if(i==0 && j==0){

            }
            else if(yCoord+j==0 || yCoord+j==height+1 || xCoord+i==0 || xCoord+i==width+1){

            }
            else if(matrixOfFields[yCoord+j][xCoord+i].getOpened()==false){

                    numberOfClosed++;

            }
        }

    }

    return numberOfClosed;

}


int Board::getWidth() {
    return width;
}
int Board::getHeight() {
    return height;
}
int Board::getMinesNumber() {
    return mineCount;
}
int Board::getFlaggedNumber() {
    return flaggedCount;
}
int Board::getUnknownNumber(){
    return unknownCount;
}
