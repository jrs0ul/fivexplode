#include <cstdlib>
#include "Grid.h"

Grid::Grid(){

    reset();
}
//--------------------------
void Grid::reset(){
    
    freespace = 100;
    for (int i = 0; i < GRIDHEIGHT; i++)
        for (int a = 0; a < GRIDWIDTH; a++){
            grid[i][a].state = IDLE;
            grid[i][a].color = 0;
        }

}
//---------------------------
void Grid::spawnFigure(unsigned col){
    if (freespace){
  
        int x = rand() % GRIDWIDTH;
        int y = rand() % GRIDHEIGHT;

        while (grid[y][x].color){
            x = rand() % GRIDWIDTH;
            y = rand() % GRIDHEIGHT;
        }

        grid[y][x].color = col;
        grid[y][x].state = NEW;
        freespace--;
    }

}
//-----------------------------
void Grid::cleanDead(){
    for (int i = 0; i < GRIDHEIGHT; i++)
        for (int a = 0; a < GRIDWIDTH; a++)
            if ((grid[i][a].state == DEAD) && (grid[i][a].color != EMPTY)){
                grid[i][a].color = EMPTY;
                grid[i][a].state = IDLE;
                freespace++;
            }
}
