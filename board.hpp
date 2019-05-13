#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "field.hpp"

#define MAX_SIZE 52


class Board {
private:
    int width;  // sirina table za igru
    int height; // visina table za igru
    int mineCount; // broj mina
    int flaggedCount; //broj flagovanih
    int unknownCount;  //broj neotvorenih
    Field matrixOfFields[MAX_SIZE][MAX_SIZE]; //matrica koja cuva infirmacije o polju
    int numberOfFlaggedAdjacentMines(int xCoord, int yCoord);
    int numberOfAdjacentMines(int xCoord, int yCoord);
    int numberOfClosedAdjacentFields(int xCoord, int yCoord);
    int flagged(int xCoord,int yCoord);

public:
    Board(int w,int h,int n_m);
    Board();
    Board(int w, int h, bool mines[(MAX_SIZE-2)*(MAX_SIZE-2)]);
    std::vector<int> open(int xCoord, int yCoord);
    std::vector<int> hintFlag();
    std::vector<int> hintOpen();
    int putFlagOrUnflag(int xCoord, int yCoord);
    int getWidth();
    int getHeight();
    int getMinesNumber();
    int getFlaggedNumber();
    int getUnknownNumber();

};





#endif
