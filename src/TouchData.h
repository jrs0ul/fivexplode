#ifndef _TOUCH_DATA
#define _TOUCH_DATA

#include "DArray.h"

struct TouchData {
    DArray<Vector3D> up;
    DArray<Vector3D> move;
    DArray<Vector3D> down;
    DArray<Vector3D> oldDown;
    
    bool allfingersup;
    
};



#endif //_TOUCH_DATA

