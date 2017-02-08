#include "FindPath.h"
#include <cmath>
#include <cstring>



//-----------------------------
void Path::destroy(){
    visited.destroy();
    parent.destroy(); 
}
//------------------------------
int Path::minival(int* val,int count){ 
    int tmp = 0;

  if (count<2) return 0;

  for (int i=1; i<count;i++)
   if(val[tmp]<val[i])
      tmp=tmp; else tmp=i;
  
  return tmp;
 
}

//-------------------------------------------------

bool Path::findPath(_Point src, _Point dest, bool** map, int width, int height){
 
    //printf("dest %d %d\n", dest.x, dest.y);
    _Point cmas[4];
    _Point pmas[4];
    int   vmas[4];
    int nodescount=0;
     
    found = false;
    
    parent.add(src);
    visited.add(src);
    

    do{    

        nodescount = 0;
        unsigned last = parent.count() - 1;
        cmas[0].x = parent[last].x;
        cmas[0].y = parent[last].y - 1;
        cmas[1].x = parent[last].x + 1;
        cmas[1].y = parent[last].y;
        cmas[2].x = parent[last].x;
        cmas[2].y = parent[last].y + 1;
        cmas[3].x = parent[last].x - 1;
        cmas[3].y = parent[last].y;


        for (int i = 0; i < 4; i++){
           
            if ((cmas[i].x > -1) && (cmas[i].y > -1)&&
                (cmas[i].x < width) && (cmas[i].y < height)&&
                (map[cmas[i].y][cmas[i].x]) 
                &&(!isVisited(cmas[i]))){
                pmas[nodescount] = cmas[i];
                nodescount++;
            }
        }
        
        if (nodescount > 0){
            for (int a=0; a < nodescount;a++)
                vmas[a] = abs(dest.x-pmas[a].x)+abs(dest.y-pmas[a].y);
            
            int mini = 0;
            if (nodescount > 1) 
                mini = minival(vmas,nodescount);
       
            _Point n;
            n.x = pmas[mini].x;
            n.y = pmas[mini].y;
            parent.add(n);
            visited.add(n);
            //printf("c %lu\n", parent.count());
        }
        else{
            parent.remove(parent.count() - 1);
            //printf("after remove c %lu\n", parent.count());
        }

        if (parent.count())
            if ((parent[parent.count()-1].x == dest.x) && (parent[parent.count() - 1].y == dest.y)){
              //  puts("found yay");
                found = true;
            }
        
    }
    while((parent.count())&&(!found));

    return found;
}
//---------------------------------------
bool Path::isVisited(_Point p){
    unsigned i = 1;
    while (((visited[i].x!=p.x)||(visited[i].y!=p.y))&&(i<visited.count()))
        i++;
    if ((visited[i].x==p.x)&&(visited[i].y==p.y)) 
        return true;
    else
        return false;
}
