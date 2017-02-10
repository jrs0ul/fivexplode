#ifndef GRID_H
#define GRID_H

#include "externals.h"

enum FigureState{NEW, IDLE, MOVING, DEAD};
enum FigureColors{EMPTY, FIRE, SLIME, GHOST, CTHULU, GOLEM, SNOWMAN, PEDOBEAR, OBELISK, BLINKIE};

struct Cell{
    unsigned color;
    FigureState state;
};

class Grid{

    int freespace;
public:
    Cell grid[GRIDHEIGHT][GRIDWIDTH];
    Grid();

    void reset();
    void spawnFigure(unsigned col);
    void cleanDead();
    int freeSquares(){return freespace;}
};



#endif //GRID_H
