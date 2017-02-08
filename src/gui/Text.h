/*
 The Disarray 
 by jrs0ul(jrs0ul ^at^ gmail ^dot^ com) 2010
 -------------------------------------------
 Text stuff
 mod. 2011.03.08
 */

#ifndef _BITMAP_TEXT_H
#define _BITMAP_TEXT_H

#include "../Colors.h"
#include "../TextureLoader.h"


//writes text
void WriteText(int x, int y,
               PicsContainer& pics, int font, const char* s,
               float scalex=1.0f, float scaley=1.0f,
               COLOR c1 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR c2 = COLOR(1.0f, 1.0f, 1.0f, 1.0f));
void WriteShadedText(int x, int y,
               PicsContainer& pics, int font, const char* s,
               float scalex=1.0f, float scaley=1.0f,
               COLOR c1 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR c2 = COLOR(1.0f, 1.0f, 1.0f, 1.0f),
               COLOR shade = COLOR(0.0, 0.0, 0.0, 1.0f));


#endif //_BITMAP_TEXT_H

