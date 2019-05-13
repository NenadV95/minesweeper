#include "field.hpp"

Field::Field():opened(false),flagged(false),hasMine(false),numberOfMines(false){

}

Field::~Field(){

}

bool Field::getOpened(){

    return opened;

}

void Field::setOpened(bool o){

    opened=o;

}

bool Field::getFlagged(){

    return flagged;

}

void Field::setFlagged(bool f){

    flagged=f;

}

bool Field::getHasMine(){

    return hasMine;

}

void Field::setHasMine(bool hm){

    hasMine=hm;

}

int Field::getNumberOfMines(){

    return numberOfMines;

}

void Field::setNumberOfMines(int nom){

    numberOfMines=nom;

}
