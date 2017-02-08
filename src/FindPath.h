#ifndef PATH_H
#define PATH_H

#include "DArray.h"

struct _Point{
    int x;
    int y;
};

class Path{

public:
    DArray<_Point> parent;
    DArray<_Point> visited;
    
    bool found;
 
    Path(){ found = false; }
    //grazina masyvo val maziausios reiksmes indeksa, count - limitas
    int minival(int* val, int count);
    bool findPath(_Point src, _Point dest, bool** map, int width, int height);
    bool isVisited(_Point p);
    void destroy();
};

#endif //PATH_H